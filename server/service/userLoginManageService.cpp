#include <errno.h>
#include <sstream>
#include <assert.h>
#include "userLoginManageService.h"
#include "register.h"
#include "loginer.h"
#include "syncClientInfo.h"


using namespace std;

static const char* IP = "127.0.0.1";
static const int MESSAGE_DISPATCHER_SERVICE_PORT = 8081;
static const int MESSAGE_MAX_LEN = 10240;

UserLoginManageService::UserLoginManageService(SqlApi& sqlApi, int saltWorkFactor)
	:_sqlApi(sqlApi),
	 _saltWorkFactor(_saltWorkFactor)
{}

UserLoginManageService::~UserLoginManageService()
{}



Status UserLoginManageService::regist(ServerContext* context, const RegistRequest* request, RegistResponse* response)
{
	int code = 0;
	string message;
	string userName = request->user_name();
	string passWord = request->pass_word();
	LOG_INFO("regist %s %s", userName.c_str(), passWord.c_str());
	
	Register myRegister(_sqlApi, _saltWorkFactor);
	int ret = myRegister.processRegist(userName, passWord, code, message);
	if(ret != 0)
	{
		LOG_ERROR("processRegist failed! errcode:%d, errmsg:%s", code, message.c_str());
	}

	context->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
    response->set_message(message);
    return Status::OK;
}


Status UserLoginManageService::login(ServerContext* context, const LoginRequest* request, LoginResponse* response)
{
 	int code = 0;
	string message;
	string userName = request->user_name();
	string passWord = request->pass_word();

	Loginer loginer(_sqlApi);
	int ret = loginer.processLogin(userName, passWord, code, message);
	if(ret != 0)
	{
		LOG_ERROR("processLogin failed! errcode:%d, errmsg:%s", code, message.c_str());
	}

	context->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
    response->set_message(message);
    return Status::OK;
 }


 Status UserLoginManageService::logout(ServerContext* context, const LogoutRequest* request, LogoutResponse* response)
 {
 	int code = 0;
	string message;
	string userName = request->user_name();
	int logoutType = request->logout_type();

	Loginer loginer(_sqlApi);
	int ret = loginer.processLogout(userName, logoutType, code, message);
	if(ret != 0)
	{
		LOG_ERROR("processLogin failed! errcode:%d, errmsg:%s", code, message.c_str());
	}

	context->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
    response->set_message(message);
    return Status::OK;
 }


 Status UserLoginManageService::syncClientInfo(ServerContext* context, const SyncClientInfoRequest* request, SyncClientInfoResponse* response)
 {
 	int code = 0;
	string message;
	string userName = request->user();
	string ip = request->ip();
	int port = request->port();

	SyncClientInfo syncClientInfo(_sqlApi);
	int ret = syncClientInfo.processSyncClientInfo(userName, ip, port, code, message);
	if(ret != 0)
	{
		LOG_ERROR("processSyncClientInfo failed! errcode:%d, errmsg:%s", code, message.c_str());
	}

	context->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
    response->set_message(message);
    return Status::OK;
 }






