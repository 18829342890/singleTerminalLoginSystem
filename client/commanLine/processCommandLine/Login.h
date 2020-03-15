#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"
#include "mylib/myLibEncrypt/base64.h"

using namespace client::commandLine;
using proto::userLoginManage::LoginRequest;
using proto::userLoginManage::LoginResponse;

extern int isLogined;
extern string userName;

class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const char* params[])
	{
		LoginRequest loginRequest;
		int ret = getLoginRequestFromParams(params, loginRequest);
		if(ret < 0)
		{
			return -1;
		}

		// ClientContext context;
		// LoginResponse loginResponse;
		// context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		// Status status = stub->login(&context, loginRequest, &loginResponse);
		// if(status.ok())
		// {
		// 	cout << loginResponse.message() << endl;
		// 	isLogined = 1;
		// 	userName = loginRequest.user_name();
		// 	return 0;
		// }
		// else
		// {
		// 	cout << "RPC failed! errcode:" << status.error_code() << ", errmsg:" << status.error_message() << endl;
		// 	return -1;
		// }
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

		string userName = params[0];
		string passWord = params[1];

		//编码userName, passWord
		char userNameEncrypted[1024] = {0};
		char passWordEncrypted[1024] = {0};
		if(base64_encode(userName.c_str(), userName.length(), userNameEncrypted) < 0)
		{
			cout << "base64_encode failed!" << endl;
			return -1;
		}
		if(base64_encode(passWord.c_str(), passWord.length(), passWordEncrypted) < 0)
		{
			cout << "base64_encode failed!" << endl;
			return -1;
		}


		
		//设置username、password
		loginRequest.set_user_name(userNameEncrypted);
		loginRequest.set_pass_word(passWordEncrypted);
		return 0;
	}

};




#endif