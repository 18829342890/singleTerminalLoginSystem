#ifndef __REGIST_H__
#define __REGIST_H__


#include "ProcessCommandLineBase.h"
#include "Enum.h"

using proto::messageReceiver::RegistRequest;
using proto::messageReceiver::RegistResponse;


class RegistCmd : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[])
	{
		RegistRequest registRequest;
		int ret = getRegistRequestFromParams(params, registRequest);
		if(ret < 0)
		{
			return -1;
		}

		ClientContext context;
		RegistResponse registResponse;
		context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		Status status = stub->regist(&context, registRequest, &registResponse);
		if(status.ok())
		{
			cout << registResponse.message() << endl;
			return 0;
		}
		else
		{
			cout << "RPC failed! errcode:" << status.error_code() << ", errmsg:" << status.error_message() << endl;
			return -1;
		}
	}


private:
	int getRegistRequestFromParams(const char* params[], RegistRequest& registRequest)
	{
		//判断参数是否合法
		int i = 0;
		for(; params[i]; ++i){}

		if(i != 2)
		{
			cout << "please input regist username password!" << endl;
			return -1;
		}

		//设置username、password
		registRequest.set_user_name(params[0]);
		registRequest.set_pass_word(params[1]);
		return 0;
	}

};




#endif