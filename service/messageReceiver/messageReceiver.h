#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/epoll.h>

#include "logrecord.h"

static const int PORT = 8080;
static const int MAXEVENTS = 64;
static const int TIMEOUT = 5000;
static const int MAX_MESSAGE_LEN = 10240;

enum E_CODE
{
	SUCCESS = 0,
	SYSTEM_ERROR = -1,
};

typedef struct eventDataBuf
{
	int fd;
	int code;
	char message[MAX_MESSAGE_LEN];
}eventDataBuf_t;


class MessageReceiver
{
public:
	int createListenSock();
	int createEpollFd();
	int epollCtl(int epollFd, int op, int fd, int events);
	int dealConnectRequest(int listenSocket, int epollFd);
	int dealReadEvent(int fd, int epollFd);
	int dealWriteEvent(int fd, int epollFd, epoll_data_t data);
	int dealClientMessage(int sendToClientFd, int code, char* message);
};



#endif