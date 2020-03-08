#include "syncClientInfo.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/repository/userLoginManageRepository.h"
#include <sstream>
#include <assert.h>

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;


SyncClientInfo::SyncClientInfo(SqlApi& sqlApi) 
		: _sqlApi(sqlApi)
{}

SyncClientInfo::~SyncClientInfo()
{}

int SyncClientInfo::processSyncClientInfo(const string& userName, const string& ip, int port, int& code, string& msg)
{
	//检查用户是否已登录
	bool isAlreadyLogined = false;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	int ret = userLoginManageRepository.getIsAlreadyLogined(userName, isAlreadyLogined);
	if(ret != 0)
	{
		LOG_ERROR("getIsAlreadyLogined failed!");
		return -1;
	}

	if(!isAlreadyLogined)
	{
		code = USER_NOT_LOGINED;
		msg = "User not logged in!";
		return 0;
	}

	//已登录则更新客户端信息
	ret = userLoginManageRepository.updateClientInfoByUserName(userName, ip, port);
	if(ret != 0)
	{
		LOG_ERROR("updateClientInfoByUserName failed!");
		code = DB_OP_FAILED;
		msg = "SYSTEM ERROR: update data to DB failed!";
		return -1;
	}

	code = SUCCESS;
	msg = "sync client info success!";
	return 0;
}














































