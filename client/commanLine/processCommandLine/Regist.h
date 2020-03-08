#ifndef __REGIST_H__
#define __REGIST_H__


#include "ProcessCommandLineBase.h"
#include "base64.h"

using namespace client::commandLine;
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

		string userName = params[0];
		string passWord = params[1];

		//加密passWord
		char passWordEncrypted[1024] = {0};
		if(base64_encode(passWord.c_str(), passWord.length(), passWordEncrypted) < 0)
		{
			cout << "base64_encode failed!" << endl;
			return -1;
		}

		//设置username、password
		registRequest.set_user_name(userName);
		registRequest.set_pass_word(passWordEncrypted);
		return 0;
	}

};




#endif