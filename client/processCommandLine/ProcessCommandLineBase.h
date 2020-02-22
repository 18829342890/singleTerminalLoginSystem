#ifndef __PROCESS_COMMAND_LENE_H__
#define __PROCESS_COMMAND_LENE_H__

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <grpcpp/grpcpp.h>
#include "messageReceiver.grpc.pb.h"

using namespace std;
using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::Status;
using proto::messageReceiver::messageReceiver;

const int MESSAGE_MAX_LEN = 10240;

class ProcessCommandLineBase
{

public:
	ProcessCommandLineBase(){};
	~ProcessCommandLineBase(){};

	/*
	 * 处理命令的公共接口
	 */
	virtual int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[]){}
};





#endif