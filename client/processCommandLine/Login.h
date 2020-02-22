#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"
#include "Enum.h"

using proto::messageReceiver::LoginRequest;
using proto::messageReceiver::LoginResponse;

class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[])
	{
		LoginRequest loginRequest;
		int ret = getLoginRequestFromParams(params, loginRequest);
		if(ret < 0)
		{
			return -1;
		}

		ClientContext context;
		LoginResponse loginResponse;
		context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		Status status = stub->login(&context, loginRequest, &loginResponse);
		if(status.ok())
		{
			cout << loginResponse.message() << endl;
			return 0;
		}
		else
		{
			cout << "RPC failed! errcode:" << status.error_code() << ", errmsg:" << status.error_message() << endl;
			return -1;
		}
		return 0;
	}


private:
	int getLoginRequestFromParams(const char* params[], LoginRequest& loginRequest)
	{
		//判断是否合法
		int i = 0;
		for(; params[i]; ++i){}

		if(i != 2)
		{
			cout << "please input login username password!" << endl;
			return -1;
		}
		
		//设置username、password
		loginRequest.set_user_name(params[0]);
		loginRequest.set_pass_word(params[1]);
		return 0;
	}

};




#endif