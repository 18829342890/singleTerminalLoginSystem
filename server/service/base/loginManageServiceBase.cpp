#include "loginManageServiceBase.h"
#include "mylib/enum/code.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;

LoginManageServiceBase::LoginManageServiceBase(const sql::Connection* mysqlConnect)
	:_code(SUCCESS),
	 _msg("success"),
	 _mysqlConnect(mysqlConnect)
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