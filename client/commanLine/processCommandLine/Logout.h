#ifndef __MY_LOGOUT_H__
#define __MY_LOGOUT_H__

#include <stdlib.h>
#include "ProcessCommandLineBase.h"
#include "mylib/enum/code.h"

using namespace client::commandLine;
using proto::messageReceiver::LogoutRequest;
using proto::messageReceiver::LogoutResponse;


class LogoutCmd : public ProcessCommandLineBase
{

public:

	int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[])
	{
		LogoutRequest logoutRequest;
		int ret = getLogoutRequestFromParams(params, logoutRequest);
		if(ret < 0)
		{
			return -1;
		}

		ClientContext context;
		LogoutResponse logoutResponse;
		context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		Status status = stub->logout(&context, logoutRequest, &logoutResponse);
		if(status.ok())
		{
			cout << logoutResponse.message() << endl;
			return 0;
		}
		else
		{
			cout << "RPC failed! errcode:" << status.error_code() << ", errmsg:" << status.error_message() << endl;
			return -1;
		}
	}
		
	int getLogoutRequestFromParams(const char* params[], LogoutRequest& logoutRequest)
	{
		//判断参数是否合法
		int i = 0;
		for(; params[i]; ++i){}

		if(i != 1)
		{
			cout << "please input logout username" << endl;
			return -1;
		}

		//设置username
		logoutRequest.set_user_name(params[0]);
		logoutRequest.set_logout_type(ADMINISTRATOR_KICK_OUT);
		return 0;
	}

};





#endif