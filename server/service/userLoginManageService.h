#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include <mysql_connection.h>
#include <hiredis/hiredis.h>
#include "mylib/mylibSql/sqlApi.h"
#include "mylib/mylibLog/logrecord.h"
#include "protos/proto/userLoginManage.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;
using proto::userLoginManage::userLoginManageService;
using proto::userLoginManage::BasicRequest;
using proto::userLoginManage::BasicResponse;
using proto::userLoginManage::RegistRequest;
using proto::userLoginManage::RegistResponse;
using proto::userLoginManage::LoginRequest;
using proto::userLoginManage::LoginResponse;
using proto::userLoginManage::LogoutRequest;
using proto::userLoginManage::LogoutResponse;
using proto::userLoginManage::HeartBeatRequest;
using proto::userLoginManage::HeartBeatResponse;
using proto::userLoginManage::KickOutUserRequest;
using proto::userLoginManage::KickOutUserResponse;

class UserLoginManageService final : public userLoginManageService::Service 
{
public:
	UserLoginManageService(const sql::Connection* mysqlConnect,const redisContext* redisConnect, int saltWorkFactor, int userLoginInfoCacheTtl);
	~UserLoginManageService();

	//注册
    Status regist(ServerContext* context, ServerReaderWriter<RegistResponse, RegistRequest>* stream);
    //登录
    Status login(ServerContext* context, ServerReaderWriter<LoginResponse, LoginRequest>* stream);
    //退出登录
    Status logout(ServerContext* context, ServerReaderWriter<LogoutResponse, LogoutRequest>* stream);
    //心跳请求
    Status heartBeat(ServerContext* context, ServerReaderWriter<HeartBeatResponse, HeartBeatRequest>* stream);
    //踢出用户请求
    Status kickOutUser(ServerContext* context, ServerReaderWriter<KickOutUserResponse, KickOutUserRequest>* stream);

private:
    const sql::Connection* _mysqlConnect;
    const redisContext* _redisConnect;
    int _saltWorkFactor;
    int _userLoginInfoCacheTtl;
};



#endif