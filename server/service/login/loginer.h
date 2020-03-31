#pragma once

#include <string>
#include "mylib/myLibEncrypt/bcrypt.h"
#include "server/service/base/loginManageServiceBase.h"
#include "server/infrastructure/include/rabbitmqClient.h"


using namespace std;
using namespace userLoginService::infrastructure;


class Loginer : public LoginManageServiceBase
{
public:
	Loginer(const sql::Connection* mysqlConnect, RabbitmqClient& rabbitmqClient, const string& exchange, const string& routeKey, const string& queueName);
	virtual ~Loginer();

	int processLogin(const string& userName, const string& passWord, const string& clientUid);
	int monitorUserLogoutEvent(const string& userName, const string& clientUid, bool& isNeedLogout);

private:
	//用户名和密码是否有效
	int isValidPassWordByUserName(const string& userName, const string& passWord, bool& isValid);
	//处理已登录设备
	int processLoginedDevice(const string& userName);
	int noticeUserLogout(const string& userName, const string& clientUid);
	//登录
	int login(const string& userName, const string&  clientUid);

private:
	RabbitmqClient& _rabbitmqClient;
	const string& _exchange;
    const string& _routeKey;
    const string& _queueName;
};




