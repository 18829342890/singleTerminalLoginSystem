#include "messageReceiver.h"
#include "cJSON.h"
#include "Enum.h"
#include <errno.h>
#include <sstream>
#include <assert.h>

using namespace std;

static const char* IP = "127.0.0.1";
static const int MESSAGE_DISPATCHER_SERVICE_PORT = 8081;
static const int MESSAGE_MAX_LEN = 10240;

MessageReceiver::MessageReceiver()
{
	_sendToMessageDispatcherServiceFd = 0;
}

MessageReceiver::~MessageReceiver()
{}


int MessageReceiver::connectTomessageDispatcherService(int epollFd)
{
	//连接注册/登录服务
	int socketFd = 0;
	int ret = connectToService(IP, MESSAGE_DISPATCHER_SERVICE_PORT, socketFd);
	if(ret < 0)
	{
		LOG_ERROR("connectToService failed!");
		return -1;
	}

	ret = epollCtl(epollFd, EPOLL_CTL_ADD, socketFd, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed!");
		return -1;
	}

	_sendToMessageDispatcherServiceFd = socketFd;
	_fdName.insert(pair<int, string>(socketFd, "MESSAGE DISPATCHER SERVICE"));
	return 0;
}

int MessageReceiver::dealClientMessage(int sendToClientFd, int code, char* message)
{
	string needSendMessage;
	getNeedSendMessage(message, sendToClientFd, needSendMessage);
	LOG_INFO("sendTo:%s, code:%d, needSendMessage:%s", getFdName(_sendToMessageDispatcherServiceFd), code, needSendMessage.c_str());

	//把信息发送给messageDispatcherService
	int ret = write(_sendToMessageDispatcherServiceFd, needSendMessage.c_str(), needSendMessage.length());
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	//获取messageDispatcherService的响应
	char response[MESSAGE_MAX_LEN] = {0};
	ret = read(_sendToMessageDispatcherServiceFd, response, sizeof(response));
	if(ret < 0)
	{
		LOG_ERROR("read failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	//把响应发给客户端
	response[ret] = '\0';
	LOG_INFO("response:%s", response);

	ret = dealServiceResponse(sendToClientFd, response);
	if(ret < 0)
	{
		LOG_ERROR("dealServiceResponse failed!");
		return -1;
	}

	return 0;
}

void MessageReceiver::getNeedSendMessage(const char* receivedMessage, int sendToClientFd, string& needSendMessage)
{
	cJSON* messageJson = cJSON_Parse(receivedMessage);

	//添加extendInfo、msgType
	cJSON* extendInfo = cJSON_CreateObject();
	cJSON_AddNumberToObject(extendInfo, "clientFd", sendToClientFd);
	cJSON_AddItemToObject(messageJson, "extendInfo", extendInfo);


	//转化为string
	std::stringstream ss;
	ss << cJSON_PrintUnformatted(messageJson);
	needSendMessage = ss.str();

	cJSON_Delete(messageJson);
	return;
}

int MessageReceiver::dealServiceResponse(int sendToClientFd, const char* response)
{
	cJSON* responseJson = cJSON_Parse(response);
	cJSON* msgItem  = cJSON_GetObjectItem(responseJson, "msg");
	cJSON* extendInfoItem  = cJSON_GetObjectItem(responseJson, "extendInfo");
	if(msgItem == NULL || extendInfoItem == NULL)
	{
		LOG_ERROR("not found msg or extendInfo item from json! json:%s", cJSON_PrintUnformatted(responseJson));
		return -1;
	}

	//获取对应服务的fd
	assert(msgItem->type == cJSON_String);
	assert(extendInfoItem->type == cJSON_Object);
	cJSON* logoutFdItem  = cJSON_GetObjectItem(extendInfoItem, "logoutFd");
	if(logoutFdItem == NULL)
	{
		LOG_ERROR("not found logoutFdItem item from json! json:%s", cJSON_PrintUnformatted(responseJson));
		return -1;
	}

	assert(logoutFdItem->type == cJSON_Number);
	string msg = msgItem->valuestring;
	int logoutFd = logoutFdItem->valueint;

	//通知已登录设备退出登录
	if(logoutFd > 0)
	{
		string logoutString = "logout";//待改进，有loginer传出消息
		LOG_INFO("sendTo:%s, message:%s", getFdName(logoutFd), logoutString.c_str());
		int ret = write(logoutFd, logoutString.c_str(),  logoutString.length());
		if(ret < 0)
		{
			LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
			return -1;
		}
	}
	
	//通知登录成功
	LOG_INFO("sendTo:%s, message:%s", getFdName(sendToClientFd), msg.c_str());
	int ret = write(sendToClientFd, msg.c_str(),  msg.length());
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return 0;
}







































