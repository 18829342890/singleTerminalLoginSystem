#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include <mysql_connection.h>
#include "mylib/mylibLog/logrecord.h"
#include "protos/proto/userLoginManage.grpc.pb.h"
#include "server/infrastructure/include/rabbitmqClient.h"

using namespace std;
using namespace userLoginService::infrastructure;

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
	UserLoginManageService(const sql::Connection* mysqlConnect, RabbitmqClient& rabbitmqClient, const string& exchange, const string& routeKey, const string& queueName, int saltWorkFactor);
	~UserLoginManageService();

	//注册
    Status regist(ServerContext* context, ServerReaderWriter<RegistResponse, RegistRequest>* stream);
    //登录
    Status login(ServerContext* context, ServerReaderWriter<LoginResponse, LoginRequest>* stream);
    //退出登录
    Status logout(ServerContext* context, ServerReaderWriter<LogoutResponse, LogoutRequest>* stream);
    //踢出用户请求
    Status kickOutUser(ServerContext* context, ServerReaderWriter<KickOutUserResponse, KickOutUserRequest>* stream);

private:
    const sql::Connection* _mysqlConnect;
    RabbitmqClient& _rabbitmqClient;
    const string& _exchange;
    const string& _routeKey;
    const string& _queueName;
    int _saltWorkFactor;
};



#endif