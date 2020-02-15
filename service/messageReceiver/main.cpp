#include "messageReceiver.h"



int main()
{
	MessageReceiver messageReceiver;

	//创建listenSocket
	int listenSocket = messageReceiver.createListenSock();
	if(listenSocket < 0)
	{
		LOG_ERROR("createListenSock failed!");
		return -1;
	}

	//创建epollFd
	int epollFd = messageReceiver.createEpollFd();
	if(epollFd < 0)
	{
		LOG_ERROR("createEpollFd failed!");
		return -1;
	}

	//把listenSocket加到epoll监控的fd中
	int ret = epollCtl(epollFd, EPOLL_CTL_ADD, listenSocket, EPOLLIN);
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
			LOG_INFO("epoll_wait timeout!");
			break;
		case -1:
			LOG_ERROR("epoll_wait failed! errno:%d, errmsg:%s", errno, strerror(errno));
			break;
		default:
			for(int index = 0; index < readyFdCount; ++index)
			{
				int readyFd = eventOuts[index].data.fd;
				if(readyFd == listenSocket && (ev_outs[index].events & EPOLLIN))
				{
					//收到连接请求
					LOG_INFO("receive connect request!");
					messageReceiver.dealConnectRequest(listenSocket, epollFd);
				}
				else
				{
					//已连接套接字准备就绪
					if(eventOuts[index].events & EPOLLIN)
					{
						messageReceiver.dealReadEvent(readyFd, epollFd);
					}
					else if(eventOuts[index].events & EPOLLOUT)
					{
						messageReceiver.dealWriteEvent(readyFd, epollFd, eventOuts[index].data);
					}
				}
			}
			break;
		}
	}







}