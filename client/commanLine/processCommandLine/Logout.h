#ifndef __MY_LOGOUT_H__
#define __MY_LOGOUT_H__

#include <stdlib.h>
#include "ProcessCommandLineBase.h"
#include "mylib/enum/code.h"


using namespace userLoginSystem::myEnum;
using namespace client::commandLine;
using proto::userLoginManage::LogoutRequest;
using proto::userLoginManage::LogoutResponse;


extern int isLogined;
extern string userName;

class LogoutCmd : public ProcessCommandLineBase
{

public:

	int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		//获取请求
		LogoutRequest logoutRequest;
		int ret = getLogoutRequestFromParams(clientUuid, logoutRequest);
		if(ret < 0)
		{
			return -1;
		}

		//发送退出登录请求
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
			isLogined = 0;
			return 0;
		}
		else
		{
			cout << "server occur error. please try again! server errmsg:" << logoutResponse.basic().msg();
			return -1;
		}
	}
		
	int getLogoutRequestFromParams(const string& clientUuid, LogoutRequest& logoutRequest)
	{
		//判断是否已登录
		if(!isLogined)
		{
			cout << "please login!" << endl;
			return -1;
		}

		//编码userName
		char userNameEncrypted[1024] = {0};
		base64_encode(userName.c_str(), userName.length(), userNameEncrypted);

		//设置username
		logoutRequest.mutable_basic()->set_uuid(clientUuid);
		logoutRequest.set_user_name(userNameEncrypted);
		return 0;
	}

};





#endif