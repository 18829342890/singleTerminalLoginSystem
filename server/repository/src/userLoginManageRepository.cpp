#include "server/repository/include/userLoginManageRepository.h"
#include <sstream>

using namespace userLoginService::repository;

int UserLoginManageRepository::getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined)
{
	//select
	std::stringstream ss;
	ss << "select count(1) from user.t_user_login_manage where FstrUserName = '" << userName << "' and FuiStatus = 1";

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
	LOG_INFO("userName:%s, status =1`s count:%d", userName.c_str(), count);

	isAlreadyLogined = count == 0 ? false : true;
	return 0;
}

int UserLoginManageRepository::getAlreadyLoginedClientInfo(const string& userName, string& ip, int& port)
{
	std::stringstream ss;
	ss << "select FstrIp,FuiPort from user.t_user_login_manage where FstrUserName = '" << userName << "' and FuiStatus = 1";

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

	if(mysqlResult.rowsCount <= 0)
	{
		LOG_ERROR("this user not logined! userName:%s", userName.c_str());
		return -1;
	}

	//获取ip、port
	assert(mysqlResult.fieldsCount == 2);
	ip = mysqlResult.mysqlRowVec[0][0];
	port = strtoul(mysqlResult.mysqlRowVec[0][1], NULL, 0);

	LOG_INFO("userName:%s already logined, FstrIp:%s, port:%d", userName.c_str(), ip.c_str(), port);
	return 0;
}


int UserLoginManageRepository::setStatusByUserName(const string& userName, int status)
{
	std::stringstream ss;
	ss << "insert into user.t_user_login_manage (FstrUserName,FuiStatus,FuiCreateTime) values ('" 
	   << userName << "', " << status << ", " << time(NULL) 
	   << ") ON DUPLICATE KEY UPDATE FstrUserName = '" 
	   << userName << "', FuiStatus = " << status;

	if(_sqlApi.insert(ss.str()) != 0)
	{
		LOG_ERROR("insert failed! insert string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}

int UserLoginManageRepository::updateClientInfoByUserName(const string& userName, const string& ip, int port)
{
	std::stringstream ss;
	ss << "update user.t_user_login_manage set FstrIp = '" << ip << "', FuiPort =" << port << " where FstrUserName = '" << userName << "'";

	int ret = _sqlApi.update(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("update failed! update string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}