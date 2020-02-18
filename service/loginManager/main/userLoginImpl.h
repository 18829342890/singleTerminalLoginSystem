#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "logrecord.h"
#include "sqlApi.h"
#include "daemon.h"




class UserLoginImpl : public DaemonBase
{
public:
	UserLoginImpl(SqlApi& sqlApi);
	virtual ~UserLoginImpl();

private:
	int dealClientMessage(int sendToClientFd, int code, char* receivedMessage);
	const char* getFdName(int fd);
	int getOperateType(const char* receivedMessage, int& operateType);
	int getResponseMessage(const char* receivedMessage, int logoutFd, int code, std::string msg, string& needSendmessage);
	int processLogin(const char* receivedMessage, int sendToClientFd, int& logoutFd, int& errcode, string& errmsg);
	int processRegist(const char* receivedMessage, int& errcode, string& errmsg);
	int processLogout(const char* receivedMessage, int& errcode, string& errmsg);


private:
	SqlApi _sqlApi;
};



#endif