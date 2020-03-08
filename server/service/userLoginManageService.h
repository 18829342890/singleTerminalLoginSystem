#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include "mylib/mylibSql/sqlApi.h"
#include "mylib/mylibLog/logrecord.h"
#include "protos/proto/messageReceiver.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using proto::messageReceiver::messageReceiver;
using proto::messageReceiver::RegistRequest;
using proto::messageReceiver::RegistResponse;
using proto::messageReceiver::LoginRequest;
using proto::messageReceiver::LoginResponse;
using proto::messageReceiver::SyncClientInfoRequest;
using proto::messageReceiver::SyncClientInfoResponse;
using proto::messageReceiver::LogoutRequest;
using proto::messageReceiver::LogoutResponse;

class UserLoginManageService final : public messageReceiver::Service 
{
public:
	UserLoginManageService(SqlApi& sqlApi, int saltWorkFactor);
	~UserLoginManageService();

	//注册
    Status regist(ServerContext* context, const RegistRequest* request, RegistResponse* response);
    //登录
    Status login(ServerContext* context, const LoginRequest* request, LoginResponse* response);
    //退出登录
    Status logout(ServerContext* context, const LogoutRequest* request, LogoutResponse* response);
    //同步客户端信息
    Status syncClientInfo(ServerContext* context, const SyncClientInfoRequest* request, SyncClientInfoResponse* response);

private:
	SqlApi _sqlApi;
    int _saltWorkFactor;
};



#endif