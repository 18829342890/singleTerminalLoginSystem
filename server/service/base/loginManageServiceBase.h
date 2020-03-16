#pragma once

#include <string>
#include <hiredis/hiredis.h>
#include <cppconn/connection.h>
#include "server/domain/userLoginCacheBO.h"

using namespace userLoginService::BO;

class LoginManageServiceBase
{
public:
	LoginManageServiceBase(const sql::Connection* mysqlConnect, const redisContext* redisConnect);
	~LoginManageServiceBase();

	int getCode();
	string getMsg();

protected:
	int getUserLoginInfo(const string& userName, UserLoginCacheBO& userLoginCache);


protected:
	int _code;
	string _msg;
	const sql::Connection* _mysqlConnect;
	const redisContext* _redisConnect;
};