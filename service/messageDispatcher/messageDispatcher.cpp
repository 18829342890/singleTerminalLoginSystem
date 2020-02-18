#include "messageDispatcher.h"
#include "Enum.h"
#include "cJSON.h"
#include <errno.h>
#include <sstream>
#include <assert.h>


static const char* IP = "127.0.0.1";
static const int LOGINER_SERVICE_PORT = 8082;
static const int MESSAGE_MAX_LEN = 10240;

template <class T>
static std::string type2str(T v) {
    std::stringstream s;
    s << v;

    return s.str();
}


MessageDispatcher::MessageDispatcher()
{
}

MessageDispatcher::~MessageDispatcher()
{}



int MessageDispatcher::connectAllHandleService(int epollFd)
{
	//连接注册/登录服务
	int socketFd = 0;
	int ret = connectToService(IP, LOGINER_SERVICE_PORT, socketFd);
	if(ret < 0)
	{
		LOG_ERROR("connectToService failed!");
		return -1;
	}

	LOG_INFO("connect to service fd:%d", socketFd);
	ret = epollCtl(epollFd, EPOLL_CTL_ADD, socketFd, EPOLLIN);
	if(ret < 0)
	{
		LOG_ERROR("epollCtl failed!");
		return -1;
	}

	_serviceFdMap.insert(pair<int, int>(LOGINER_SERVICE, socketFd));
	_fdName.insert(pair<int, string>(socketFd, "LOGINER SERVICE"));
	return 0;
}

int MessageDispatcher::dealClientMessage(int sendToClientFd, int code, char* receivedMessage)
{
	//获取消息类型，并根据消息类型把消息发送给不同的服务
	int serviceFd = 0;
	string needSendMessage;
	int ret = getServiceFdAndMessage(receivedMessage, serviceFd, needSendMessage);
	if(ret != 0)
	{
		LOG_ERROR("getServiceFdAndMessage failed");
		return -1;
	}

	LOG_INFO("request:%s", needSendMessage.c_str());
	ret = write(serviceFd, needSendMessage.c_str(), needSendMessage.length());
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	//获取对应服务的响应
	char response[MESSAGE_MAX_LEN] = {0};
	ret = read(serviceFd, response, sizeof(response));
	if(ret < 0)
	{
		LOG_ERROR("read failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	//把响应发给客户端
	response[ret] = '\0';
	LOG_INFO("response:%s", response);

	LOG_INFO("sendTo:%s, message:%s", getFdName(sendToClientFd), response);
	ret = write(sendToClientFd, response, strlen(response));
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	return 0;

}


int MessageDispatcher::getServiceFdAndMessage(const char* receivedMessage, int& serviceFd, string& needSendMessage)
{
	//获取处理的消息类型
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* messageTypeItem  = cJSON_GetObjectItem(receivedMessageJson, "messageType");
	if(messageTypeItem == NULL)
	{
		LOG_ERROR("not found messageType item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	//获取对应服务的fd
	assert(messageTypeItem->type == cJSON_Number);
	int messageType = messageTypeItem->valueint;
	switch(messageType)
	{
	case REGIST:
	case LOGIN:
	case LOGOUT:
		if(dealLogin(receivedMessageJson, messageType, serviceFd, needSendMessage) != 0)
		{
			LOG_ERROR("dealLogin failed");
			cJSON_Delete(receivedMessageJson);
			return -1;
		}

		cJSON_Delete(receivedMessageJson);
		return 0;
	default:
		LOG_ERROR("messageType in json is illegal! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	return 0;
}

int MessageDispatcher::dealLogin(cJSON* receivedMessageJson, int messageType, int& serviceFd, string& needSendMessage)
{
	//获取登录服务的fd
	map<int, int>::iterator iter = _serviceFdMap.find(LOGINER_SERVICE);
	if(iter == _serviceFdMap.end())
	{
		LOG_ERROR("not found fd! service enum:%d", LOGINER_SERVICE);
		return -1;
	}

	serviceFd = iter->second;

	//添加操作类型
	cJSON_AddNumberToObject(receivedMessageJson, "operateType", messageType);

	//转化为string
	std::stringstream ss;
	ss << cJSON_PrintUnformatted(receivedMessageJson);
	needSendMessage = ss.str();
	return 0;
}







































