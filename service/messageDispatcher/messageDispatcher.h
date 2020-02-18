#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<map>
#include<string>
#include "cJSON.h"
#include "daemon.h"

using namespace std;



enum HANDLE_SERVICE
{
	LOGINER_SERVICE = 1,   //注册、登录处理服务
};

class MessageDispatcher : public DaemonBase
{
public:
	MessageDispatcher();
	virtual ~MessageDispatcher();
	int dealClientMessage(int sendToClientFd, int code, char* message);
	//连接所有具体的处理服务
	int connectAllHandleService(int epollFd);
	int getServiceFdAndMessage(const char* receivedMessage, int& serviceFd, string& needSendMessage);
	int dealLogin(cJSON* receivedMessageJson, int messageType, int& serviceFd, string& needSendMessage);

private:
	std::map<int, int> _serviceFdMap;

};



#endif