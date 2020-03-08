#include <sstream>
#include <assert.h>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "loginer.h"
#include "code.h"
#include "logrecord.h"
#include "userPasswordRepository.h"
#include "userLoginManageRepository.h"
#include "protos/proto/messageReceiver.grpc.pb.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;
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
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	ret = userLoginManageRepository.getIsAlreadyLogined(userName, isAlreadyLogined);
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
	if(login(userName) != 0)
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
	bool isAlreadyLogined = false;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	int ret = userLoginManageRepository.getIsAlreadyLogined(userName, isAlreadyLogined);
	if(ret != 0)
	{
		LOG_ERROR("getIsAlreadyLogined failed!");
		return -1;
	}

	//已退出登录
	if(!isAlreadyLogined)
	{
		code = SUCCESS;
		msg = "logout success!";
		LOG_INFO("already logout! just return success.");
		return 0;
	}

	//未退出登录，则退出登录
	if(logoutType == ADMINISTRATOR_KICK_OUT)
	{
		//如果是管理员踢出用户，则先通知客户端退出登录
		ret = noticeLoginedClientLogout(userName);
		if(ret != 0)
		{
			code = DB_OP_FAILED;
			msg = "notice user logout failed!";
			LOG_ERROR("noticeLoginedClientLogout failed!");
			return -1;
		}
	}

	//设置为退出登录状态
	ret = userLoginManageRepository.setStatusByUserName(userName, LOGOUT);
	if(ret != 0)
	{
		LOG_ERROR("setStatusByUserName failed!");
		return -1;
	}

	code = SUCCESS;
	msg = "logout success!";
	LOG_INFO("logout success!");
	return 0;
}

int Loginer::isValidUserNameAndPassWord(const string& userName, const string& passWord, bool& isValid)
{
	//获取该用户加密用的salt
	char salt[BCRYPT_HASHSIZE];
	UserPasswordRepository userPasswordRepository(_sqlApi);
	int ret = userPasswordRepository.getSaltByUserName(userName, salt);
	if(ret != 0)
	{
		LOG_ERROR("getSaltByUserName failed!");
		return -1;
	}

	//加密密码
	char passWordHash[BCRYPT_HASHSIZE];
	ret = bcrypt_hashpw(userName.c_str(), salt, passWordHash);
	if(ret != 0)
	{
		LOG_ERROR("bcrypt_hashpw failed!");
		return -1;
	}

	ret = userPasswordRepository.isValidUserNameAndPassWord(userName, passWordHash, isValid);
	if(ret != 0)
	{
		LOG_ERROR("isValidUserNameAndPassWord failed!");
		return -1;
	}

	return 0;
}

int Loginer::login(const string& userName)
{
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	return userLoginManageRepository.setStatusByUserName(userName, LOGINED);
}

int Loginer::noticeLoginedClientLogout(const string& userName)
{
	//获取已登录设备的信息
	int clientPort;
	string clientIp;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	int ret = userLoginManageRepository.getAlreadyLoginedClientInfo(userName, clientIp, clientPort);
	if(ret != 0)
	{
		LOG_ERROR("getAlreadyLoginedClientInfo failed! userName:%s", userName.c_str());
		return -1;
	}

	//通知退出登录
	noticeLogout(clientIp, clientPort);
	return 0;
}

string Loginer::readFile2String(const string& fileName) 
{
	std::ifstream ifs(fileName);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

int Loginer::noticeLogout(const string& clientIp, int clientPort)
{
	std::stringstream address;
	address << clientIp << ":" << clientPort;

	auto cacert = readFile2String("../../auth/ca.crt");
  	auto options = grpc::SslCredentialsOptions();
  	options.pem_root_certs = cacert;
  	auto creds = grpc::SslCredentials(options);

	std::shared_ptr<Channel> channel = grpc::CreateChannel(address.str().c_str(), creds);
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















































