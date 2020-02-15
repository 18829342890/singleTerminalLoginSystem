#include "messageReceiver.h"
#include <errno.h>

static const char* IP = "127.0.0.1";

int MessageReceiver::createListenSock()
{
	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocket < 0)
	{
		LOG_ERROR("socket failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr(IP);
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


int MessageReceiver::createEpollFd()
{
	int epollFd = epoll_create(256);
	if(epollFd < 0)
	{
		LOG_ERROR("epoll_create failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return epollFd;
}

int MessageReceiver::epollCtl(int epollFd, int op, int fd, int events)
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

int MessageReceiver::dealConnectRequest(int listenSocket, int epollFd)
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
	int ret = epollCtl(epollFd, EPOLL_CTL_ADD, linkedSocket, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return 0;
}

int MessageReceiver::dealReadEvent(int fd, int epollFd)
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
		epollCtl(epollFd, EPOLL_CTL_DEL, fd, 0);
		close(fd);
		free(eventDataBuf);
	}
	else
	{
		eventDataBuf->code = SUCCESS;
		eventDataBuf->message[ret] = '\0';
		LOG_INFO("recv message:%s", eventDataBuf->message);

		//修改为写事件，并把读取的数据传给写者
		struct epoll_event ev;
		ev.events = EPOLLOUT;
		ev.data.ptr = eventDataBuf;
		epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
	}

	return 0;
}

int MessageReceiver::dealWriteEvent(int fd, int epollFd, epoll_data_t data)
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

	free(eventDataBuf);
	return 0;
}


int MessageReceiver::dealClientMessage(int sendToClientFd, int code, char* message)
{
	LOG_INFO("sendToClientFd:%d, code:%d, message:%s", sendToClientFd, code, message);
	printf("sendToClientFd:%d, code:%d, message:%s", sendToClientFd, code, message);

}







































