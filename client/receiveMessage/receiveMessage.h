#ifndef __CLIENT_MESSAGE_RECEIVER_H__
#define __CLIENT_MESSAGE_RECEIVER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <grpcpp/grpcpp.h>
#include "mylib/mylibLog/logrecord.h"
#include "protos/proto/messageReceiver.grpc.pb.h"

using namespace std;
using grpc::Status;
using grpc::ServerContext;
using proto::messageReceiver::messageReceiver;
using proto::messageReceiver::NoticeClientLogoutRequest;
using proto::messageReceiver::NoticeClientLogoutResponse;

class MessageReceiver final : public messageReceiver::Service 
{
public:
	//接收服务端通知退出的消息
    Status noticeClientLogout(ServerContext* context, const NoticeClientLogoutRequest* request, NoticeClientLogoutResponse* response);
};



#endif