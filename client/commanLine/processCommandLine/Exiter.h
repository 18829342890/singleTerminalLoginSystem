#ifndef __MY_EXIT_H__
#define __MY_EXIT_H__

#include <stdlib.h>
#include "ProcessCommandLineBase.h"
#include "mylib/enum/code.h"


using namespace userLoginSystem::myEnum;
using namespace client::commandLine;
using namespace userLoginSystem::myEnum;
using proto::userLoginManage::LogoutRequest;
using proto::userLoginManage::LogoutResponse;

extern int isLogined;
extern string userName;

class Exiter : public ProcessCommandLineBase
{

public:

	int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		//如果未登录，直接退出
		if(!isLogined) exit(0);

		//如果已经登录，则发送退出登录请求
		LogoutRequest logoutRequest;
		char userNameEncrypted[1024] = {0};
		base64_encode(userName.c_str(), userName.length(), userNameEncrypted);
		logoutRequest.mutable_basic()->set_uuid(clientUuid);
		logoutRequest.set_user_name(userNameEncrypted);

		ClientContext context;
		LogoutResponse logoutResponse;
		Status status = stub->logout(&context, logoutRequest, &logoutResponse);
		if(!status.ok())
		{
			cout << "rpc failed! " << status.error_code() << ": " << status.error_message() << endl;
			return -1;
		}

		//判断响应
		if(logoutResponse.basic().code() == SUCCESS)
		{
			cout << logoutResponse.basic().msg() << endl;
			exit(0);
		}
		else
		{
			cout << "server occur error. please try again! server errmsg:" << logoutResponse.basic().msg();
			return -1;
		}
	}

};





#endif