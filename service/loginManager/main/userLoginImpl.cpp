#include "userLoginImpl.h"
#include "register.h"
#include "loginer.h"
#include "cJSON.h"
#include "Enum.h"
#include "code.h"
#include <errno.h>
#include <sstream>
#include <assert.h>


UserLoginImpl::UserLoginImpl(SqlApi& sqlApi)
	:_sqlApi(sqlApi)
{}

UserLoginImpl::~UserLoginImpl()
{}


int UserLoginImpl::dealClientMessage(int sendToClientFd, int code, char* receivedMessage)
{
	LOG_INFO("receivedMessage:%s", receivedMessage);

	//获取操作类型
	string errmsg;
	int errcode = 0;
	int logoutFd = -1;
	int operateType = 0;
	int ret = getOperateType(receivedMessage, operateType);
	if(ret != 0)
	{
		LOG_ERROR("getOperateType failed!");
		errmsg = "SYSTEM ERROR:Message format error!";
		goto sendMessage;
	}

	switch(operateType)
	{
	case REGIST:
		ret = processRegist(receivedMessage, errcode, errmsg);
		if(ret != 0)
		{
			errcode = SYSTEM_ERROR;
			errmsg = "SYSTEM ERROR: process regist occur unexpect error!";
			goto sendMessage;
		}
		break;
	case LOGIN:
		ret = processLogin(receivedMessage, sendToClientFd, logoutFd, errcode, errmsg);
		if(ret != 0)
		{
			errcode = SYSTEM_ERROR;
			errmsg = "SYSTEM ERROR: process login occur unexpect error!";
			goto sendMessage;
		}
		break;
	case LOGOUT:
		ret = processLogout(receivedMessage, errcode, errmsg);
		if(ret != 0)
		{
			errcode = SYSTEM_ERROR;
			errmsg = "SYSTEM ERROR: process logout occur unexpect error!";
			goto sendMessage;
		}
		break;
	default:
		LOG_ERROR("operateType is illegal! operateType:%d", operateType);
		errcode = SYSTEM_ERROR;
		errmsg = "SYSTEM ERROR: operateType is illegal!";
		goto sendMessage;
	}

sendMessage:
	string needSendmessage;
	ret = getResponseMessage(receivedMessage, logoutFd, errcode, errmsg, needSendmessage);
	if(ret != 0)
	{
		errcode = SYSTEM_ERROR;
		errmsg = "SYSTEM ERROR: get need send message failed!";
	}

	LOG_INFO("need send message:%s", needSendmessage.c_str());
	ret = write(sendToClientFd, needSendmessage.c_str(), needSendmessage.length());
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	sleep(2);//TODO 

	return 0;

}

int UserLoginImpl::getResponseMessage(const char* receivedMessage, int logoutFd, int code, string msg, string& needSendmessage)
{
	//创建json
	cJSON* root = cJSON_CreateObject();

	//code and msg
	cJSON_AddNumberToObject(root, "code", code);
	cJSON_AddStringToObject(root, "msg", msg.c_str());

	//extendInfo
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* extendInfoItem  = cJSON_GetObjectItem(receivedMessageJson, "extendInfo");
	if(extendInfoItem == NULL)
	{
		LOG_ERROR("not found extendInfo item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	cJSON_AddNumberToObject(extendInfoItem, "logoutFd", logoutFd);
	cJSON_AddItemToObject(root, "extendInfo", extendInfoItem);

	//转化为string
	std::stringstream ss;
	ss << cJSON_PrintUnformatted(root);
	needSendmessage = ss.str();

	cJSON_Delete(root);
	return 0;
}

int UserLoginImpl::getOperateType(const char* receivedMessage, int& operateType)
{
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* messageTypeItem  = cJSON_GetObjectItem(receivedMessageJson, "operateType");
	if(messageTypeItem == NULL)
	{
		LOG_ERROR("not found operateType item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	//获取对应服务的fd
	assert(messageTypeItem->type == cJSON_Number);
	operateType = messageTypeItem->valueint;
	return 0;
}

int UserLoginImpl::processLogin(const char* receivedMessage, int sendToClientFd, int& logoutFd, int& errcode, string& errmsg)
{
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* messageItem = cJSON_GetObjectItem(receivedMessageJson, "message");
	cJSON* extendInfoItem = cJSON_GetObjectItem(receivedMessageJson, "extendInfo");
	if(messageItem == NULL || extendInfoItem == NULL)
	{
		LOG_ERROR("not found message or extendInfo item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(messageItem->type == cJSON_Object);
	assert(extendInfoItem->type == cJSON_Object);

	cJSON* userNameItem = cJSON_GetObjectItem(messageItem, "userName");
	cJSON* passWordItem = cJSON_GetObjectItem(messageItem, "passWord");
	cJSON* clientFdItem = cJSON_GetObjectItem(extendInfoItem, "clientFd");
	if(userNameItem == NULL || passWordItem == NULL || clientFdItem == NULL)
	{
		LOG_ERROR("not found userName or passWord or clientFd item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(userNameItem->type == cJSON_String);
	assert(passWordItem->type == cJSON_String);
	assert(clientFdItem->type == cJSON_Number);
	char* userName = userNameItem->valuestring;
	char* passWord = passWordItem->valuestring;
	int clientFd = clientFdItem->valueint;

	Loginer loginer(_sqlApi);
	int ret = loginer.processLogin(userName, passWord, clientFd, logoutFd, errcode, errmsg);
	if(ret != 0)
	{
		LOG_ERROR("loginer.processLogin failed!");
		return -1;
	}

	return 0;
}

int UserLoginImpl::processLogout(const char* receivedMessage, int& errcode, string& errmsg)
{
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* messageItem = cJSON_GetObjectItem(receivedMessageJson, "message");
	if(messageItem == NULL)
	{
		LOG_ERROR("not found message item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(messageItem->type == cJSON_Object);
	cJSON* userNameItem = cJSON_GetObjectItem(messageItem, "userName");
	if(userNameItem == NULL)
	{
		LOG_ERROR("not found userName item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(userNameItem->type == cJSON_String);
	char* userName = userNameItem->valuestring;

	Loginer loginer(_sqlApi);
	int ret = loginer.processLogout(userName, errcode, errmsg);
	if(ret != 0)
	{
		LOG_ERROR("loginer.processLogout failed!");
		return -1;
	}

	return 0;
}

int UserLoginImpl::processRegist(const char* receivedMessage, int& errcode, string& errmsg)
{
	LOG_INFO("get in processRegist.............")
	cJSON* receivedMessageJson = cJSON_Parse(receivedMessage);
	cJSON* messageItem = cJSON_GetObjectItem(receivedMessageJson, "message");
	if(messageItem == NULL)
	{
		LOG_ERROR("not found message item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(messageItem->type == cJSON_Object);
	cJSON* userNameItem = cJSON_GetObjectItem(messageItem, "userName");
	cJSON* passWordItem = cJSON_GetObjectItem(messageItem, "passWord");
	if(userNameItem == NULL || passWordItem == NULL)
	{
		LOG_ERROR("not found userName or passWord item from json! json:%s", cJSON_PrintUnformatted(receivedMessageJson));
		return -1;
	}

	assert(userNameItem->type == cJSON_String);
	assert(passWordItem->type == cJSON_String);
	char* userName = userNameItem->valuestring;
	char* passWord = passWordItem->valuestring;

	Register myRegister(_sqlApi);
	int ret = myRegister.processRegist(userName, passWord, errcode, errmsg);
	if(ret != 0)
	{
		LOG_ERROR("myRegister.processRegist failed!");
		return -1;
	}

	return 0;
}










































