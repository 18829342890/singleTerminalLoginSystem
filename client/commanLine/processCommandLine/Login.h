#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"
#include "mylib/myLibEncrypt/base64.h"
#include "mylib/enum/code.h"

using namespace userLoginSystem::myEnum;
using namespace client::commandLine;
using proto::userLoginManage::LoginRequest;
using proto::userLoginManage::LoginResponse;

extern int isLogined;
extern string userName;

class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		//获取请求
		LoginRequest loginRequest;
		int ret = getLoginRequestFromParams(params, clientUuid, loginRequest);
		if(ret < 0)
		{
			return -1;
		}

		//发送登录请求
		ClientContext context;
		std::shared_ptr<ClientReaderWriter<LoginRequest, LoginResponse> > stream(stub->login(&context));
		stream->Write(loginRequest);
		stream->WritesDone();

		//获取响应
		LoginResponse loginResponse;
		stream->Read(&loginResponse);
		if(loginResponse.basic().code() == SUCCESS)
		{
			cout << loginResponse.basic().msg() << endl;
			isLogined = 1;

			char userNameDecoded[1024];
			base64_decode(loginRequest.user_name().c_str(), loginRequest.user_name().length(), userNameDecoded);
			userName = userNameDecoded;
			return 0;
		}
		else
		{
			cout << "server occur error. please try again! server errmsg:" << loginResponse.basic().msg();
			return -1;
		}
	}


private:
	int getLoginRequestFromParams(const char* params[], const string& clientUuid, LoginRequest& loginRequest)
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
		base64_encode(userName.c_str(), userName.length(), userNameEncrypted);
		base64_encode(passWord.c_str(), passWord.length(), passWordEncrypted);

		//设置username、password
		loginRequest.mutable_basic()->set_uuid(clientUuid);
		loginRequest.set_user_name(userNameEncrypted);
		loginRequest.set_pass_word(passWordEncrypted);
		return 0;
	}

};




#endif