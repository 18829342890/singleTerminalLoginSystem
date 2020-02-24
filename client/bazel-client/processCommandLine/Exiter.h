#ifndef __MY_EXIT_H__
#define __MY_EXIT_H__

#include "ProcessCommandLineBase.h"
#include "code.h"
#include <stdlib.h>


using proto::messageReceiver::LogoutRequest;
using proto::messageReceiver::LogoutResponse;

extern int isLogined;
extern string userName;

class Exiter : public ProcessCommandLineBase
{

public:

	int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[])
	{
		//如果未登录，直接退出
		if(!isLogined) exit(0);

		//如果已经登录，则发送退出客户端请求
		LogoutRequest logoutRequest;
		logoutRequest.set_user_name(userName);
		logoutRequest.set_logout_type(USER_LOGOUT);

		ClientContext context;
		LogoutResponse logoutResponse;
		context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		Status status = stub->logout(&context, logoutRequest, &logoutResponse);
		if(status.ok())
		{
			cout << logoutResponse.message() << endl;
			exit(0);
			return 0;
		}
		else
		{
			cout << "RPC failed! errcode:" << status.error_code() << ", errmsg:" << status.error_message() << endl;
			return -1;
		}
	}

};





#endif