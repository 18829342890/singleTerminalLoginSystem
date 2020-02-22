#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include "sqlApi.h"
#include "logrecord.h"
#include "messageReceiver.pb.h"
#include "messageReceiver.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using proto::messageReceiver::messageReceiver;
using proto::messageReceiver::RegistRequest;
using proto::messageReceiver::RegistResponse;

class MessageReceiver final : public messageReceiver::Service 
{
public:
	MessageReceiver(SqlApi& sqlApi);
	~MessageReceiver();
	// int dealClientMessage(int sendToClientFd, int code, char* message);
 //    int connectTomessageDispatcherService(int epollFd);
 //    void getNeedSendMessage(const char* receivedMessage, int sendToClientFd, std::string& needSendMessage);
 //    int dealServiceResponse(int sendToClientFd, const char* response);

    Status regist(ServerContext* context, const RegistRequest* request, RegistResponse* reply);

private:
	int _sendToMessageDispatcherServiceFd;
	SqlApi _sqlApi;
};



#endif