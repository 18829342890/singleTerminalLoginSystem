#ifndef __REGIST_H__
#define __REGIST_H__


#include "ProcessCommandLineBase.h"
#include "mylib/myLibEncrypt/base64.h"
#include "mylib/enum/code.h"


using namespace userLoginSystem::myEnum;
using namespace client::commandLine;
using proto::userLoginManage::RegistRequest;
using proto::userLoginManage::RegistResponse;


class RegistCmd : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		//获取请求
		RegistRequest registRequest;
		int ret = getRegistRequestFromParams(params, registRequest);
		if(ret < 0)
		{
			return -1;
		}

		//发送注册请求
		ClientContext context;
		std::shared_ptr<ClientReaderWriter<RegistRequest, RegistResponse> > stream(stub->regist(&context));
		stream->Write(registRequest);
		stream->WritesDone();

		//获取响应
		RegistResponse registResponse;
		stream->Read(&registResponse);
		if(registResponse.basic().code() == SUCCESS)
		{
			cout << registResponse.basic().msg() << endl;
			return 0;
		}
		else
		{
			cout << "server occur error. please try again! server errmsg:" << registResponse.basic().msg();
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

		//编码username、passWord
		char userNameEncrypted[1024] = {0};
		char passWordEncrypted[1024] = {0};
		base64_encode(userName.c_str(), userName.length(), userNameEncrypted);
		base64_encode(passWord.c_str(), passWord.length(), passWordEncrypted);

		//设置username、password
		registRequest.set_user_name(userNameEncrypted);
		registRequest.set_pass_word(passWordEncrypted);
		return 0;
	}

};




#endif