#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include "sqlApi.h"
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

class MessageReceiver final : public messageReceiver::Service 
{
public:
	MessageReceiver(SqlApi& sqlApi);
	~MessageReceiver();

    Status regist(ServerContext* context, const RegistRequest* request, RegistResponse* response);
    Status login(ServerContext* context, const LoginRequest* request, LoginResponse* response);
    Status logout(ServerContext* context, const LogoutRequest* request, LogoutResponse* response);
    Status syncClientInfo(ServerContext* context, const SyncClientInfoRequest* request, SyncClientInfoResponse* response);

private:
	int _sendToMessageDispatcherServiceFd;
	SqlApi _sqlApi;
};



#endif