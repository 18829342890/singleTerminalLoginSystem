#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "logrecord.h"
#include "daemon.h"


class MessageReceiver : public DaemonBase
{
public:
	MessageReceiver();
	~MessageReceiver();
	int dealClientMessage(int sendToClientFd, int code, char* message);
    int connectTomessageDispatcherService(int epollFd);
    void getNeedSendMessage(const char* receivedMessage, int sendToClientFd, std::string& needSendMessage);
    int dealServiceResponse(int sendToClientFd, const char* response);

private:
	int _sendToMessageDispatcherServiceFd;
};



#endif