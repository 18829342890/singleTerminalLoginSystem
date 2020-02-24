#include "syncClientInfo.h"
#include "code.h"
#include "logrecord.h"
#include <sstream>
#include <assert.h>


SyncClientInfo::SyncClientInfo(SqlApi& sqlApi) 
		: _sqlApi(sqlApi)
{}

SyncClientInfo::~SyncClientInfo()
{}

int SyncClientInfo::processSyncClientInfo(const string& userName, const string& ip, int port, int& code, string& msg)
{
	//检查用户是否已登录
	bool isAlreadyLogined = false;
	int ret = getIsAlreadyLogined(userName, isAlreadyLogined);
	if(!isAlreadyLogined)
	{
		code = USER_NOT_LOGINED;
		msg = "User not logged in!";
		return 0;
	}

	//已登录则更新客户端信息
	ret = updateUserClientInfo(userName, ip, port);
	if(ret != 0)
	{
		LOG_ERROR("updateUserClientInfo failed!");
		code = DB_OP_FAILED;
		msg = "SYSTEM ERROR: update data to DB failed!";
		return -1;
	}

	code = SUCCESS;
	msg = "sync client info success!";
	return 0;
}

int SyncClientInfo::getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined)
{
	std::stringstream ss;
	ss << "select count(1) from user.t_user_login where FstrUserName = '" << userName << "' and FuiStatus = 1";

	//select
	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	//获取结果
	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	//判断是否已登录
	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 1);
	int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);

	isAlreadyLogined = count == 0 ? false : true;
	return 0;
}


int SyncClientInfo::updateUserClientInfo(const string& userName, const string& ip, int port)
{
	std::stringstream ss;
	ss << "update user.t_user_login set FstrIp = '" << ip << "', FuiPort =" << port << " where FstrUserName = '" << userName << "'";

	int ret = _sqlApi.update(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("update failed! update string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}














































