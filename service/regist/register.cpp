#include "register.h"
#include "code.h"
#include "logrecord.h"
#include <sstream>
#include <assert.h>
#include <grpcpp/grpcpp.h>


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

Register::Register(SqlApi& sqlApi) 
		: _sqlApi(sqlApi)
{}

Register::~Register()
{}

int Register::processRegist(const string& userName, const string& passWord, int& code, string& msg)
{
	//校验是否已经注册
	bool isAlreadyRegistFlag = false;
	int ret = isAlreadyRegist(userName, isAlreadyRegistFlag);
	if(ret != 0)
	{
		LOG_INFO("isAlreadyRegist failed! userName:%s", userName.c_str());
		code = DB_OP_FAILED;
		msg = "system error: insert data into DB failed!";
		return -1;
	}

	if(isAlreadyRegistFlag)
	{
		LOG_INFO("[%s] already regist!", userName.c_str());
		code = ALREADY_REGIST;
		string errmsg = userName + " already regist!";
		msg = errmsg;
		return 0;
	}

	//入库
	if(regist(userName, passWord) != 0)
	{
		code = DB_OP_FAILED;
		msg = "system error: insert data into DB failed!";
		return -1;
	}

	code = SUCCESS;
	msg = "regist success!";
	return 0;
}

int Register::isAlreadyRegist(const string& userName, bool& isAlreadyRegistFlag)
{
	std::stringstream ss;
	ss << "select count(1) from user.t_user_password where FstrUserName = '" << userName << "'";

	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 1);
	int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	LOG_INFO("userName:%s, count(1):%d", userName.c_str(), count);

	isAlreadyRegistFlag = count == 0 ? false : true;
	return 0;
}

int Register::regist(const string& userName, const string& passWord)
{
	std::stringstream ss;
	ss << "insert into user.t_user_password (FstrUserName, FstrPassWord, FuiCreateTime) values ('" << userName << "' ,'" << passWord << "', " << time(NULL) << ")";

	if(_sqlApi.insert(ss.str()) != 0)
	{
		LOG_ERROR("insert failed! insert string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}














