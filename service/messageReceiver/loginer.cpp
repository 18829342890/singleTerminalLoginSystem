#include "loginer.h"
#include "code.h"
#include "logrecord.h"
#include <sstream>
#include <assert.h>


Loginer::Loginer(SqlApi& sqlApi) 
		: _sqlApi(sqlApi)
{}

Loginer::~Loginer()
{}

int Loginer::processLogin(const string& userName, const string& passWord, int sendToClientFd, int& code, string& msg)
{
	//判断用户名和密码是否有效
	bool isValid = false;
	int ret = isValidUserNameAndPassWord(userName, passWord, isValid);
	if(ret < 0)
	{
		LOG_ERROR("isValidUserNameAndPassWord failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		code = SYSTEM_ERROR;
		msg = "select DB data failed!";
		return -1;
	}

	if(!isValid)
	{
		LOG_ERROR("userName or passWord error! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		code = USERNAME_OR_PASSWD_ERROR;
		msg = "userName or passWord error!";
		return -1;
	}

	//判断是否已为登录状态
	if( !isAlreadyLogined(userName) )
	{
		//登录
		if(login(userName, passWord, sendToClientFd) != 0)
		{
			LOG_ERROR("login failed! userName:%s, passWord:%s, sendToClientFd:%d", userName.c_str(), passWord.c_str(), sendToClientFd);
			code = DB_OP_FAILED;
			msg = "system error: insert data into DB failed!";
			return -1;
		}
	}
	else
	{
		//其它已登录设备强制退出，然后登录该设备
		LOG_INFO("userName already logined! Force other logged in devices to exit, and then log in to the device! userName:%s", userName.c_str());

		//获取已登录设备对应的fd
		unsigned long long fuiId = 0;
		int alreadyLoginedClientFd = 0;
		ret = getAlreadyLoginedClientFd(userName, alreadyLoginedClientFd, fuiId);
		if(ret != 0)
		{
			LOG_ERROR("getAlreadyLoginedClientFd failed! userName:%s", userName.c_str());
			code = SYSTEM_ERROR;
			msg = "system error: Get already logined client fd failed!";
			return -1;
		}

		//通知已登录设备退出登录
		ret = noticeLogout(alreadyLoginedClientFd);
		if(ret != 0)
		{
			LOG_ERROR("noticeLogout failed! userName:%s, sendToClientFd:%s", userName.c_str(), sendToClientFd);
			code = SYSTEM_ERROR;
			msg = "system error: notice already logined devices logout failed!";
			return -1;
		}

		//设置已登录的记录为退出登录状态
		ret = setLogoutStatusThenLogin(fuiId, userName, passWord, sendToClientFd);
		if(ret != 0)
		{
			LOG_ERROR("setLogoutStatusThenLogin failed! userName:%s, FuiId:%d", userName.c_str(), fuiId);
			code = SYSTEM_ERROR;
			msg = "system error: update DB data failed!";
			return -1;
		}
	}

	code = SUCCESS;
	msg = "login success!";
	LOG_INFO("userName:%s, passWord:%s login success!", userName.c_str(), passWord.c_str());
	return 0;
}

int Loginer::isValidUserNameAndPassWord(const string& userName, const string& passWord, bool& isValid)
{
	std::stringstream ss;
	ss << "select count(1) from user.t_user_password where FstrUserName = '" << userName << "' and FstrPassWord = '" << passWord << "'";

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

	//判断是否有效
	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 1);
	int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	isValid = count == 0 ? false : true;

	LOG_INFO("userName:%s, passWord:%s, isValid:%d", userName.c_str(), passWord.c_str(), isValid);
	return 0;
}

bool Loginer::isAlreadyLogined(const string& userName)
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
	LOG_INFO("userName:%s, status =1 count:%d", userName.c_str(), count);

	return count == 0 ? false : true;
}

int Loginer::login(const string& userName, const string& passWord, int sendToClientFd)
{
	std::stringstream ss;
	ss << "insert into user.t_user_login (FstrUserName,FstrPassWord,FuiFd,FuiStatus,FuiCreateTime) values ('" 
	   << userName << "', '" << passWord << "', " << sendToClientFd << ", 1, " << time(NULL) << ")";

	if(_sqlApi.insert(ss.str()) != 0)
	{
		LOG_ERROR("insert failed! insert string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}


int Loginer::getAlreadyLoginedClientFd(const string& userName, int& sendToClientFd, unsigned long long& fuiId)
{
	std::stringstream ss;
	ss << "select FuiId,FuiFd from user.t_user_login where FstrUserName = '" << userName << "' and FuiStatus = 1";

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

	//获取fd
	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 2);
	int id = strtoull(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	int fd = strtoul(mysqlResult.mysqlRowVec[0][1], NULL, 0);
	fuiId = id;
	sendToClientFd = fd;

	LOG_INFO("userName:%s, FuiId:%d, send to client fd:%d", userName.c_str(), fuiId, fd);
	return 0;
}

int Loginer::noticeLogout(int fd)
{
	const char* message = "logout";
	int ret = write(fd, message, strlen(message));
	if(ret < 0)
	{
		LOG_ERROR("write failed! errno:%d, errmsg:%s", errno, strerror(errno));
		return -1;
	}

	//TODO 等待响应
	LOG_INFO("notice %d logout success!", fd);
	return 0;
}

int Loginer::setLogoutStatusThenLogin(unsigned long long fuiId, const string& userName, const string& passWord, int sendToClientFd)
{
	//TODO  事务
	std::stringstream ss;
	ss << "update user.t_user_login set FuiStatus = 2 where FuiId = " << fuiId;

	int ret = _sqlApi.update(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("update failed! update string:%s", ss.str().c_str());
		return -1;
	}

	return login(userName, passWord, sendToClientFd);
}















































