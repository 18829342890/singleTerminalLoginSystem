#include "loginer.h"
#include "code.h"
#include "logrecord.h"
#include <sstream>
#include <assert.h>
#include <grpcpp/grpcpp.h>
#include "messageReceiver.pb.h"
#include "messageReceiver.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::Status;
using grpc::ServerContext;
using proto::messageReceiver::messageReceiver;
using proto::messageReceiver::NoticeClientLogoutRequest;
using proto::messageReceiver::NoticeClientLogoutResponse;


Loginer::Loginer(SqlApi& sqlApi) 
		: _sqlApi(sqlApi)
{}

Loginer::~Loginer()
{}

int Loginer::processLogin(const string& userName, const string& passWord, int& code, string& msg)
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
	bool isAlreadyLogined = false;
	ret = getIsAlreadyLogined(userName, isAlreadyLogined);
	if(isAlreadyLogined)
	{
		//其它已登录设备强制退出，然后登录该设备
		LOG_INFO("userName already logined! Force other logged in devices to exit, and then log in to the device! userName:%s", userName.c_str());
		ret = noticeLoginedClientLogout(userName);
		if(ret != 0)
		{
			LOG_ERROR("noticeLoginedClientLogout failed! userName:%s", userName.c_str());
			code = SYSTEM_ERROR;
			msg = "system error: notice other user logout failed!";
			return -1;
		}
	}

	//登录
	if(login(userName, passWord) != 0)
	{
		LOG_ERROR("login failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		code = DB_OP_FAILED;
		msg = "system error: insert data into DB failed!";
		return -1;
	}

	code = SUCCESS;
	msg = "login success!";
	LOG_INFO("userName:%s, passWord:%s login success!", userName.c_str(), passWord.c_str());
	return 0;
}

int Loginer::processLogout(const string& userName, int logoutType, int& code, string& msg)
{
	std::stringstream ss;
	ss << "select FuiId from user.t_user_login where FstrUserName = '" << userName << "' and FuiStatus = 1";

	//select
	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		code = DB_OP_FAILED;
		msg = "SYSTEM ERROR: select data from DB failed!";
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	//获取结果
	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	//已退出登录
	if(mysqlResult.rowsCount <= 0)
	{
		code = SUCCESS;
		msg = "logout success!";
		LOG_INFO("already logout! just return success.");
		return 0;
	}

	//未退出登录，则退出登录
	if(logoutType == ADMINISTRATOR_KICK_OUT)
	{
		//如果是管理员踢出用户，则通知客户端退出登录
		ret = noticeLoginedClientLogout(userName);
		if(ret != 0)
		{
			code = DB_OP_FAILED;
			msg = "notice user logout failed!";
			LOG_ERROR("noticeLoginedClientLogout failed!");
			return -1;
		}
	}
	
	assert(mysqlResult.fieldsCount == 1);
	int fuiId = strtoull(mysqlResult.mysqlRowVec[0][0], NULL, 0);

	std::stringstream s;
	s << "update user.t_user_login set FuiStatus = 2 where FuiId = " << fuiId;

	ret = _sqlApi.update(s.str());
	if(ret != 0)
	{
		code = DB_OP_FAILED;
		msg = "SYSTEM ERROR: update data to DB failed!";
		LOG_ERROR("update failed! update string:%s", s.str().c_str());
		return -1;
	}

	code = SUCCESS;
	msg = "logout success!";
	LOG_INFO("logout success!");
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

int Loginer::getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined)
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

	isAlreadyLogined = count == 0 ? false : true;
	return 0;
}

int Loginer::login(const string& userName, const string& passWord)
{
	std::stringstream ss;
	ss << "insert into user.t_user_login (FstrUserName,FstrPassWord,FuiStatus,FuiCreateTime) values ('" 
	   << userName << "', '" << passWord << "', 1, " << time(NULL) 
	   << ") ON DUPLICATE KEY UPDATE FstrUserName = '" 
	   << userName << "', FstrPassWord = '" << passWord << "', FuiStatus = 1";

	if(_sqlApi.insert(ss.str()) != 0)
	{
		LOG_ERROR("insert failed! insert string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}


int Loginer::getAlreadyLoginedClientInfo(const string& userName, string& ip, int& port)
{
	std::stringstream ss;
	ss << "select FstrIp,FuiPort from user.t_user_login where FstrUserName = '" << userName << "' and FuiStatus = 1";

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
	ip = mysqlResult.mysqlRowVec[0][0];
	port = strtoul(mysqlResult.mysqlRowVec[0][1], NULL, 0);

	LOG_INFO("userName:%s already logined, FstrIp:%s, port:%d", userName.c_str(), ip.c_str(), port);
	return 0;
}

int Loginer::noticeLoginedClientLogout(const string& userName)
{
	//获取已登录设备的信息
	int clientPort;
	string clientIp;
	int ret = getAlreadyLoginedClientInfo(userName, clientIp, clientPort);
	if(ret != 0)
	{
		LOG_ERROR("getAlreadyLoginedClientInfo failed! userName:%s", userName.c_str());
		return -1;
	}

	//通知退出登录
	noticeLogout(clientIp, clientPort);
	return 0;
}

int Loginer::noticeLogout(const string& clientIp, int clientPort)
{
	std::stringstream address;
	address << clientIp << ":" << clientPort;

	ChannelArguments channelArgs;
	channelArgs.SetCompressionAlgorithm(GRPC_COMPRESS_GZIP);
	std::shared_ptr<Channel> channel = grpc::CreateCustomChannel(address.str().c_str(), grpc::InsecureChannelCredentials(), channelArgs);
	std::shared_ptr<messageReceiver::Stub> stub = messageReceiver::NewStub(channel);
	if(!stub)
	{
		printf("connect to client failed! address:%s", address.str().c_str());
		return -1;
	}

	NoticeClientLogoutRequest noticeClientLogoutRequest;
	noticeClientLogoutRequest.set_message("该账号在另一台设备登录，退出当前登录!");

	ClientContext context;
	NoticeClientLogoutResponse noticeClientLogoutResponse;
	context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
	Status status = stub->noticeClientLogout(&context, noticeClientLogoutRequest, &noticeClientLogoutResponse);
	if(status.ok())
	{
		LOG_INFO("notice another client logout success.");
		return 0;
	}
	else
	{
		LOG_ERROR("notice another client logout failed!");
		return -1;
	}
}















































