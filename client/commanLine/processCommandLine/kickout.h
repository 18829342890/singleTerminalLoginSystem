#pragma 

#include "ProcessCommandLineBase.h"
#include "mylib/myLibEncrypt/base64.h"
#include "mylib/enum/code.h"


using namespace userLoginSystem::myEnum;
using namespace client::commandLine;
using proto::userLoginManage::KickOutUserRequest;
using proto::userLoginManage::KickOutUserResponse;


class KickoutUserCmd : public ProcessCommandLineBase
{
public:
	int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		//获取请求
		KickOutUserRequest kickoutUserRequest;
		int ret = getKickoutUserRequestFromParams(params, kickoutUserRequest);
		if(ret < 0)
		{
			return -1;
		}

		//发送踢出用户请求
		ClientContext context;
		KickOutUserResponse kickOutUserResponse;
		Status status = stub->kickOutUser(&context, kickoutUserRequest, &kickOutUserResponse);
		if(!status.ok())
		{
			cout << "rpc failed! " << status.error_code() << ": " << status.error_message() << endl;
			return -1;
		}

		//判断响应
		if(kickOutUserResponse.basic().code() == SUCCESS)
		{
			cout << kickOutUserResponse.basic().msg() << endl;
			return 0;
		}
		else
		{
			cout << "server occur error. please try again! server errmsg:" << kickOutUserResponse.basic().msg();
			return -1;
		}
	}


private:
	int getKickoutUserRequestFromParams(const char* params[], KickOutUserRequest& kickoutUserRequest)
	{
		int i = 0;
		for(; params[i]; ++i){}

		if(i != 1)
		{
			cout << "please input logout username" << endl;
			return -1;
		}

		string userName = params[0];

		//编码userName
		char userNameEncrypted[1024] = {0};
		base64_encode(userName.c_str(), userName.length(), userNameEncrypted);

		//设置username
		kickoutUserRequest.set_user_name(userNameEncrypted);
		return 0;
	}

};