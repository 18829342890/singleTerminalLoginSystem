#include <errno.h>
#include <sstream>
#include <assert.h>

#include "userLoginManageService.h"
#include "mylib/myLibEncrypt/base64.h"
#include "server/service/regist/register.h"
#include "server/service/login/loginer.h"
#include "server/service/logout/logout.h"
#include "server/service/heartBeat/heartBeat.h"
#include "server/service/kickout/kickoutUser.h"


using namespace std;


UserLoginManageService::UserLoginManageService(SqlApi& sqlApi, redisContext* redisConnect, int saltWorkFactor, int userLoginInfoCacheTtl)
	:_sqlApi(sqlApi),
	 _redisConnect(redisConnect),
	 _saltWorkFactor(_saltWorkFactor),
	 _userLoginInfoCacheTtl(userLoginInfoCacheTtl)
{}

UserLoginManageService::~UserLoginManageService()
{}



Status UserLoginManageService::regist(ServerContext* context, ServerReaderWriter<RegistResponse, RegistRequest>* stream)
{
	//解码用户名和密码
	RegistRequest request;
	stream->Read(&request);

	string userName = request.user_name();
	string passWord = request.pass_word();
	LOG_INFO("regist %s %s", userName.c_str(), passWord.c_str());

	char userNameDecoded[1024];
	char passWordDecoded[1024];
	base64_decode(userName.c_str(), userName.length(), userNameDecoded);
	base64_decode(passWord.c_str(), passWord.length(), passWordDecoded);
	LOG_INFO("regist %s %s", userNameDecoded, passWordDecoded);
	
	//处理注册请求
	Register myRegister(_sqlApi, _redisConnect, _saltWorkFactor);
	int ret = myRegister.processRegist(userNameDecoded, passWordDecoded);
	if(ret != 0)
	{
		LOG_ERROR("processRegist failed! errcode:%d, errmsg:%s", myRegister.getCode(), myRegister.getMsg().c_str());
	}

	//返回注册处理结果
	RegistResponse response;
    response.mutable_basic()->set_code(myRegister.getCode());
    response.mutable_basic()->set_msg(myRegister.getMsg());
    stream->Write(response);
    return Status::OK;
}


Status UserLoginManageService::login(ServerContext* context, ServerReaderWriter<LoginResponse, LoginRequest>* stream)
{
	//解码用户名和密码
	LoginRequest request;
	stream->Read(&request);

	string userName = request.user_name();
	string passWord = request.pass_word();
	string clientUid = request.basic().uuid();
	LOG_INFO("login %s %s %s", userName.c_str(), passWord.c_str(), clientUid.c_str());

	char userNameDecoded[1024];
	char passWordDecoded[1024];
	base64_decode(userName.c_str(), userName.length(), userNameDecoded);
	base64_decode(passWord.c_str(), passWord.length(), passWordDecoded);
	LOG_INFO("login %s %s", userNameDecoded, passWordDecoded);

	//处理登录请求
	Loginer loginer(_sqlApi, _redisConnect, _userLoginInfoCacheTtl);
	int ret = loginer.processLogin(userNameDecoded, passWordDecoded, clientUid);
	if(ret != 0)
	{
		LOG_ERROR("processLogin failed! errcode:%d, errmsg:%s", loginer.getCode(), loginer.getMsg().c_str());
	}

	//先返回登录处理结果
	LoginResponse response;
	response.mutable_basic()->set_code(loginer.getCode());
    response.mutable_basic()->set_msg(loginer.getMsg());
    stream->Write(response);

    //再缓存用户登录信息,并设置过期时间
    loginer.cacheUserLoginInfo(userNameDecoded, clientUid);
    return Status::OK;
 }


 Status UserLoginManageService::logout(ServerContext* context, ServerReaderWriter<LogoutResponse, LogoutRequest>* stream)
 {
 	//获取用户名和客户端uuid
 	LogoutRequest request;
	stream->Read(&request);

	string userName = request.user_name();
	string clientUid = request.basic().uuid();
	LOG_INFO("logout %s %s", userName.c_str(), clientUid.c_str());

	char userNameDecoded[1024];
	base64_decode(userName.c_str(), userName.length(), userNameDecoded);
	LOG_INFO("logout %s", userNameDecoded);


	//处理退出登录请求
	Logout logout(_sqlApi, _redisConnect, _userLoginInfoCacheTtl);
	int ret = logout.processLogout(userNameDecoded, clientUid);
	if(ret != 0)
	{
		LOG_ERROR("processLogin failed! errcode:%d, errmsg:%s", logout.getCode(), logout.getMsg().c_str());
	}

	//先返回处理结果
	LogoutResponse response;
	response.mutable_basic()->set_code(logout.getCode());
    response.mutable_basic()->set_msg(logout.getMsg());
    stream->Write(response);

    //再缓存用户退出登录信息,并设置过期时间
    logout.cacheUserLogoutInfo(userNameDecoded, clientUid);
    return Status::OK;
 }


 Status UserLoginManageService::heartBeat(ServerContext* context, ServerReaderWriter<HeartBeatResponse, HeartBeatRequest>* stream)
 {
 	//获取用户名和客户端uuid
 	HeartBeatRequest request;
	stream->Read(&request);

	string userName = request.user_name();
	string clientUid = request.basic().uuid();
	char userNameDecoded[1024];
	base64_decode(userName.c_str(), userName.length(), userNameDecoded);

	//处理心跳请求
	int clientOperation = 0;
	HeartBeat heartBeat(_sqlApi, _redisConnect, _userLoginInfoCacheTtl);
	heartBeat.processHeartBeat(userNameDecoded, clientUid, clientOperation);

	//先返回处理结果
	HeartBeatResponse response;
	response.mutable_basic()->set_code(heartBeat.getCode());
    response.mutable_basic()->set_msg(heartBeat.getMsg());
    response.set_operation(clientOperation);
    stream->Write(response);

    //再更新缓存的生存时间
    heartBeat.updateUserLoginInfoCacheTtl(userNameDecoded);
    return Status::OK;
 }


Status UserLoginManageService::kickOutUser(ServerContext* context, ServerReaderWriter<KickOutUserResponse, KickOutUserRequest>* stream)
{
 	//获取用户名
 	KickOutUserRequest request;
	stream->Read(&request);

	string userName = request.user_name();
	LOG_INFO("kickOut %s", userName.c_str());

	char userNameDecoded[1024];
	base64_decode(userName.c_str(), userName.length(), userNameDecoded);
	LOG_INFO("kickOut %s", userNameDecoded);

	//处理踢出用户请求
	KickoutUser kickoutUser(_sqlApi, _redisConnect, _userLoginInfoCacheTtl);
	int ret = kickoutUser.processkickout(userNameDecoded);
	if(ret != 0)
	{
		LOG_ERROR("processkickout failed! errcode:%d, errmsg:%s", kickoutUser.getCode(), kickoutUser.getMsg().c_str());
	}

	//先返回处理结果
	KickOutUserResponse response;
	response.mutable_basic()->set_code(kickoutUser.getCode());
    response.mutable_basic()->set_msg(kickoutUser.getMsg());
    stream->Write(response);

    //再缓存用户退出登录信息,并设置过期时间
    kickoutUser.cacheUserLogoutInfo(userNameDecoded);
    return Status::OK;
}






