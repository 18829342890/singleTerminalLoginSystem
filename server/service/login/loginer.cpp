#include <sstream>
#include <assert.h>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "loginer.h"
#include "mylib/enum/code.h"
#include "mylib/cJSON/cJSON.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/repository/include/userPasswordRepository.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::BO;
using namespace userLoginService::repository;






Loginer::Loginer(const sql::Connection* mysqlConnect, RabbitmqClient& rabbitmqClient, const string& exchange, const string& routeKey, const string& queueName)
		: LoginManageServiceBase(mysqlConnect),
		  _rabbitmqClient(rabbitmqClient),
		  _exchange(exchange),
	 	  _routeKey(routeKey),
	 	  _queueName(queueName)
{}

Loginer::~Loginer()
{}


int Loginer::processLogin(const string& userName, const string& passWord, const string& clientUid)
{
	//判断密码是否有效
	bool isValid = false;
	int ret = isValidPassWordByUserName(userName, passWord, isValid);
	if(ret < 0)
	{
		LOG_ERROR("isValidAndPassWordByUserName failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		return -1;
	}

	if(!isValid)
	{
		LOG_ERROR("userName or passWord error! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		_code = USERNAME_OR_PASSWD_ERROR;
		_msg = "userName or passWord error!";
		return -1;
	}

	//判断是否登录，如果已有其它设备登录，通知已登录设备退出登录
	ret = processLoginedDevice(userName);
	if(ret < 0)
	{
		LOG_ERROR("processLoginedDevice failed! userName:%s", userName.c_str());
		return -1;
	}

	//登录
	if(login(userName, clientUid) != 0)
	{
		LOG_ERROR("login failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		return -1;
	}

	_code = SUCCESS;
	_msg = "login success!";
	LOG_INFO("userName:%s, clientUid:%s login success!", userName.c_str(), clientUid.c_str());
	return 0;
}

int Loginer::monitorUserLogoutEvent(const string& userName, const string& clientUid, bool& isNeedSendMsg, int& code, string& msg)
{
	isNeedSendMsg = false;
	code = SUCCESS;
	msg = "";
	
	//获取消息
	LOG_INFO("begin consumer...  queueName:%s", _queueName.c_str());
	std::vector<std::string> vecRecvMsg;
    int ret = _rabbitmqClient.consumer(_queueName, vecRecvMsg, 1);
    if(ret != 0)
    {
    	LOG_ERROR("consumer message from MQ failed!");
    	return -1;
    }

    LOG_INFO("consumer message:%s", vecRecvMsg[0].c_str());

    //解析消息
    cJSON* noticeMessageJson = cJSON_Parse(vecRecvMsg[0].c_str());
    cJSON* userNameItem  = cJSON_GetObjectItem(noticeMessageJson, "userName");
    cJSON* clinetUidItem  = cJSON_GetObjectItem(noticeMessageJson, "clinetUid");
    cJSON* messageTypeItem  = cJSON_GetObjectItem(noticeMessageJson,  "messageType");
    if(userNameItem == NULL || clinetUidItem == NULL || messageTypeItem == NULL ||
       userNameItem->type != cJSON_String || clinetUidItem->type != cJSON_String || messageTypeItem->type != cJSON_Number)
    {
    	LOG_ERROR("noticeMessage json is error! noticeMessage:%s", vecRecvMsg[0].c_str());
    	return -1;
    }

    LOG_INFO("userName:%s, userNameItem:%s,", userName.c_str(), userNameItem->valuestring);
    LOG_INFO("clientUid:%s, clinetUidItem:%s,", clientUid.c_str(), clinetUidItem->valuestring);
    if(userName == userNameItem->valuestring)
    {
    	LOG_INFO("userName == userNameItem->valuestring");
    }
    if(clientUid == clinetUidItem->valuestring)
    {
    	LOG_INFO("clientUid == clinetUidItem->valuestring");
    }

    if(userName == userNameItem->valuestring && clientUid == clinetUidItem->valuestring)
    {
    	isNeedSendMsg = true;
    	getNoticeUserMessage(messageTypeItem->valueint, code, msg);
    }

    return 0;
}

void Loginer::getNoticeUserMessage(int messageType, int& code, string& msg)
{
	code = messageType;

	switch(code)
	{
		case OTHER_DEVICE_LOGINED_JUST_LOGOUT: 
			msg = "您的账户已在另一台设备登录，退出当前登录!";
			break;
		case KICKOUT_BY_MANAGER_JUST_LOGOUT:
			msg = "您的账户已被管理员踢出登录，退出当前登录!";
			break;
		default:
			msg = "";
			break;
	}
}

int Loginer::isValidPassWordByUserName(const string& userName, const string& passWord, bool& isValid)
{
	//获取该用户加密用的salt
	char salt[BCRYPT_HASHSIZE];
	UserPasswordRepository userPasswordRepository(_mysqlConnect);
	int ret = userPasswordRepository.selectSaltByUserName(userName, salt);
	if(ret != 0)
	{
		LOG_ERROR("getSaltByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	//加密密码
	char passWordHash[BCRYPT_HASHSIZE];
	ret = bcrypt_hashpw(passWord.c_str(), salt, passWordHash);
	if(ret != 0)
	{
		LOG_ERROR("bcrypt_hashpw failed!");
		_code = SYSTEM_ERROR;
		_msg = "system error: bcrypt failed!";
		return -1;
	}

	ret = userPasswordRepository.isValidPassWordByUserName(userName, passWordHash, isValid);
	if(ret != 0)
	{
		LOG_ERROR("isValidAndPassWordByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	return 0;
}

int Loginer::processLoginedDevice(const string& userName)
{
	//获取该用户的登录信息
	UserLoginManageBO userLoginManage;
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	int ret = userLoginManageRepository.select(userName, userLoginManage);
	if(ret != 0)
	{
		LOG_ERROR("userLoginManageRepository.select failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	//判断是否已有设备处于登录状态
	if(userLoginManage.getStatus() != LOGINED)
	{
		return 0;
	}

	//如果已有设备处于登录状态，则通知该设备退出登录
	return noticeUserLogout(userName, userLoginManage.getClientUid());
}

int Loginer::noticeUserLogout(const string& userName, const string& clientUid)
{
	//获取通知的消息
	string noticeMessage = getNoticeLogoutMsg(userName, clientUid, OTHER_DEVICE_LOGINED_JUST_LOGOUT);

	//发出通知
	int ret = _rabbitmqClient.publish(noticeMessage, _exchange, _routeKey);
	if(ret != 0)
	{
		LOG_ERROR("publish message failed! userName:%s, clientUid:%s, noticeMessage:%s", userName.c_str(), clientUid.c_str(), noticeMessage.c_str());
		_code = PUBLISH_MSG_FAILED;
		_msg = "system error: publish message failed!";
		return -1;
	}

	LOG_INFO("publish message success! noticeMessage:%s", noticeMessage.c_str());
	return 0;
}

int Loginer::login(const string& userName, const string& clientUid)
{
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	int ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, clientUid, LOGINED);
	if(ret != 0)
	{
		LOG_ERROR("updateClientUidAndStatusByUserName failed! userName:%s, clientUid:%s, status:%d", userName.c_str(), clientUid.c_str(), LOGINED);
		_code = DB_OP_FAILED;
		_msg = "system error: insert or update data into DB failed!";
		return -1;
	}

	return 0;
}






