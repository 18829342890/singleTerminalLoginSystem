#pragma once

#include <string>
#include <hiredis/hiredis.h>
#include "mylib/mylibSql/sqlApi.h"


class LoginManageServiceBase
{
public:
	LoginManageServiceBase(const SqlApi& sqlApi, const redisContext* redisConnect);
	~LoginManageServiceBase();

	int getCode();
	string getMsg();

protected:
	


protected:
	int _code;
	string _msg;
	const SqlApi& _sqlApi;
	const redisContext* _redisConnect;
};