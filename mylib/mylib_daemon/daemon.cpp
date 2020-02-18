#include "daemon.h"
#include "code.h"
#include <sys/stat.h>
#include <sstream>

template <class T>
static std::string type2str(T v) {
    std::stringstream s;
    s << v;

    return s.str();
}

DaemonBase::DaemonBase()
	:_clientNum(0)
{}

DaemonBase::~DaemonBase()
{}

void DaemonBase::daemonInit()
{
    // shield some signals
    signal(SIGALRM, SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGHUP,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    // fork child process
    if (fork()) exit(0);

    // creates  a new session
    if (setsid() == -1)  exit(1);

    // close
	close(0);
	close(1);
	close(2);

    //chdir("/");
    umask(0);
    LOG_INFO("DaemonInit success");
    return;
}

const char* DaemonBase::getFdName(int fd)
{
	map<int, string>::iterator iter = _fdName.find(fd);
	if(iter == _fdName.end())
	{
		LOG_ERROR("not found fd! fd:%d", fd);
		return "";
	}

	return iter->second.c_str();
}

int DaemonBase::createListenSock(const char* ip, int port)
{
	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocket < 0)
	{
		LOG_ERROR("socket failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if(bind(listenSocket, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		LOG_ERROR("bind failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	if(listen(listenSocket, 64) < 0)
	{
		LOG_ERROR("listen failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return listenSocket;
}

//获取服务端连接
int DaemonBase::connectToService(const char* ip, int port, int& socketFd)
{
    //创建套接字
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
    	LOG_ERROR("socket create failed! errno:%d, errmsg: %s", errno, strerror(errno))
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    int ret = connect(socketFd, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0)
    {
        LOG_ERROR("connect failed! errno:%d, errmsg: %s", errno, strerror(errno))
        close(socketFd);
        return -1;
    }

    return 0;
}



int DaemonBase::createEpollFd()
{
	int epollFd = epoll_create(256);
	if(epollFd < 0)
	{
		LOG_ERROR("epoll_create failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return epollFd;
}

int DaemonBase::epollCtl(int epollFd, int op, int fd, int events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;

	int ret = epoll_ctl(epollFd, op, fd, &ev);
	if(ret < 0)
	{
		LOG_ERROR("epoll_ctl failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return 0;
}

int DaemonBase::dealConnectRequest(int listenSocket, int epollFd)
{
	struct sockaddr_in client;
	socklen_t addrlen = sizeof(client);

	//accept
	int linkedSocket = accept(listenSocket, (struct sockaddr*)&client, &addrlen);
	if(linkedSocket < 0)
	{
		LOG_ERROR("accept failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	// //设置为非阻塞
	// if (0 != socket_set_nonblock(linkedSocket))
 //    {
 //        cloud_logprint("socket_set_nonblock failed! ERROR: %s\n", strerror(errno));
 //        goto err_clean;
 //    }

	//把linkedSocket加入epoll监控
	LOG_INFO("new linkedSocket:%d", linkedSocket);
	int ret = epollCtl(epollFd, EPOLL_CTL_ADD, linkedSocket, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	_fdName.insert(pair<int, string>(linkedSocket, "CLIENT " +type2str(++_clientNum)));
	return 0;
}

int DaemonBase::dealReadEvent(int fd, int epollFd)
{
	eventDataBuf_t* eventDataBuf = (eventDataBuf_t*)malloc(sizeof(eventDataBuf_t));
	memset(eventDataBuf, '\0', sizeof(eventDataBuf_t));

	//设置fd
	eventDataBuf->fd = fd;

	//读取数据
	int ret = read(fd, eventDataBuf->message, sizeof(eventDataBuf->message));
	if(ret < 0)
	{
		LOG_ERROR("read failed! ret:%d, errno:%d, errmsg:%s", ret, errno, strerror(errno));
		const char* errmsg = "SYSTEM_ERROR: read system call failed!";
		eventDataBuf->code = SYSTEM_ERROR;
		strcpy(eventDataBuf->message, errmsg);
		return -1;
	}
	else if(0 == ret)
	{
		//对端已关闭连接, 取消对该fd的监控,并释放资源
		ret = epollCtl(epollFd, EPOLL_CTL_DEL, fd, 0);
		if(ret < 0)
		{
			LOG_ERROR("epoll_ctl failed! ret:%d, errno:%d, errmsg:%s", ret, errno, strerror(errno));
		}

		close(fd);
		free(eventDataBuf);
		eventDataBuf = NULL;
	}
	else
	{
		eventDataBuf->code = SUCCESS;
		eventDataBuf->message[ret] = '\0';
		LOG_INFO("remote:%s, recv message:%s", getFdName(fd), eventDataBuf->message);

		//修改为写事件，并把读取的数据传给写者
		struct epoll_event ev;
		ev.events = EPOLLOUT;
		ev.data.ptr = eventDataBuf;
		ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
		if(ret < 0)
		{
			LOG_ERROR("epoll_ctl failed! ret:%d, errno:%d, errmsg:%s", ret, errno, strerror(errno));
		}
	}

	return 0;
}

int DaemonBase::dealWriteEvent(int readyFd, int epollFd, epoll_data_t data)
{
	//获取读取的数据
	eventDataBuf_t* eventDataBuf = (eventDataBuf_t*)data.ptr;
	int sendToClientFd = eventDataBuf->fd;
	int code = eventDataBuf->code;
	char* message = eventDataBuf->message;

	//处理客户端消息
	int ret = dealClientMessage(sendToClientFd, code, message);
	if(ret < 0)
	{
		LOG_ERROR("dealClientMessage failed! ret:%d", ret);
		free(eventDataBuf);
		return -1;
	}

	//释放读取数据时malloc的资源
	free(eventDataBuf);
	eventDataBuf = NULL;

	//修改为读事件
	ret = epollCtl(epollFd, EPOLL_CTL_MOD, sendToClientFd, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return 0;
}

int DaemonBase::dealClientMessage(int sendToClientFd, int code, char* message)
{
	return 0;
}


