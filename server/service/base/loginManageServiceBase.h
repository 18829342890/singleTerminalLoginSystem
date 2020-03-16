#pragma once

#include <string>
#include <hiredis/hiredis.h>
#include "mylib/mylibSql/sqlApi.h"
#include "server/domain/userLoginCacheBO.h"

using namespace userLoginService::BO;

class LoginManageServiceBase
{
public:
	LoginManageServiceBase(const SqlApi& sqlApi, const redisContext* redisConnect);
	~LoginManageServiceBase();

	int getCode();
	string getMsg();

protected:
	int getUserLoginInfo(const string& userName, UserLoginCacheBO& userLoginCache);


protected:
	int _code;
	string _msg;
	const SqlApi& _sqlApi;
	const redisContext* _redisConnect;
};