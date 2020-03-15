#include "loginManageServiceBase.h"
#include "mylib/enum/code.h"

using namespace userLoginSystem::myEnum;

LoginManageServiceBase::LoginManageServiceBase(const SqlApi& sqlApi, const redisContext* redisConnect)
	:_code(SUCCESS),
	 _msg("success"),
	 _sqlApi(sqlApi),
	 _redisConnect(redisConnect)
{}


LoginManageServiceBase::~LoginManageServiceBase()
{}


int LoginManageServiceBase::getCode()
{
	return _code;
}

string LoginManageServiceBase::getMsg()
{
	return _msg;
}
