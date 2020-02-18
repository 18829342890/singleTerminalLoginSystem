#ifndef __DAEMON_H__
#define __DAEMON_H__


#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<string>
#include<map>
#include "logrecord.h"

using namespace std;

static const int MAX_MESSAGE_LEN = 10240;

typedef struct eventDataBuf
{
	int fd;
	int code;
	char message[MAX_MESSAGE_LEN];
}eventDataBuf_t;

class DaemonBase
{
public:
	DaemonBase();
	virtual ~DaemonBase();
	void daemonInit();
	int createListenSock(const char* ip, int port);
	int connectToService(const char* ip, int port, int& socketFd);
	int createEpollFd();
	int epollCtl(int epollFd, int op, int fd, int events);
	int dealConnectRequest(int listenSocket, int epollFd);
	int dealReadEvent(int fd, int epollFd);
	int dealWriteEvent(int fd, int epollFd, epoll_data_t data);
	const char* getFdName(int fd);

protected:
	virtual int dealClientMessage(int sendToClientFd, int code, char* message);

protected:
	int _clientNum;
	std::map<int, std::string> _fdName;

};










#endif
