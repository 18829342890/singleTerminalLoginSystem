#include "userLoginImpl.h"
#include <errno.h>

static const int PORT = 8082;
static const char* IP = "127.0.0.1";
static const int MAXEVENTS = 64;
static const int TIMEOUT = 500;

int main()
{
	//初始化数据库
	SqlApi sqlApi;
	int ret = sqlApi.connectMysql();
	if(ret < 0)
	{
		LOG_ERROR("connectMysql failed!");
		return -1;
	}

	//创建listenSocket
	UserLoginImpl userLoginImpl(sqlApi);
	int listenSocket = userLoginImpl.createListenSock(IP, PORT);
	if(listenSocket < 0)
	{
		LOG_ERROR("createListenSock failed!");
		return -1;
	}

	//创建epollFd
	int epollFd = userLoginImpl.createEpollFd();
	if(epollFd < 0)
	{
		LOG_ERROR("createEpollFd failed!");
		return -1;
	}
	LOG_INFO("epollFd:%d", epollFd);

	//把listenSocket加到epoll监控的fd中
	ret = userLoginImpl.epollCtl(epollFd, EPOLL_CTL_ADD, listenSocket, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed!");
		return -1;
	}

	//监听事件，并处理事件
	struct epoll_event eventOuts[MAXEVENTS];
	while(1)
	{
		int readyFdCount = 0;
		switch(readyFdCount = epoll_wait(epollFd, eventOuts, MAXEVENTS, TIMEOUT))
		{
		case 0:
			//timeout
			break;
		case -1:
			LOG_ERROR("epoll_wait failed! errno:%d, errmsg:%s", errno, strerror(errno));
			break;
		default:
			for(int index = 0; index < readyFdCount; ++index)
			{
				int readyFd = eventOuts[index].data.fd;
				if(readyFd == listenSocket && (eventOuts[index].events & EPOLLIN))
				{
					//收到连接请求
					LOG_INFO("receive connect request!");
					userLoginImpl.dealConnectRequest(listenSocket, epollFd);
				}
				else
				{
					//已连接套接字准备就绪
					if(eventOuts[index].events & EPOLLIN)
					{
						userLoginImpl.dealReadEvent(readyFd, epollFd);
					}
					else if(eventOuts[index].events & EPOLLOUT)
					{
						userLoginImpl.dealWriteEvent(readyFd, epollFd, eventOuts[index].data);
					}
				}
			}
			break;
		}
	}







}