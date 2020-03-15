#ifndef __REGIST_H__
#define __REGIST_H__


#include "ProcessCommandLineBase.h"
#include "mylib/myLibEncrypt/base64.h"

using namespace client::commandLine;
using proto::userLoginManage::RegistRequest;
using proto::userLoginManage::RegistResponse;


class RegistCmd : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const char* params[])
	{
		RegistRequest registRequest;
		int ret = getRegistRequestFromParams(params, registRequest);
		if(ret < 0)
		{
			return -1;
		}

		ClientContext context;
		std::shared_ptr<ClientReaderWriter<RegistRequest, RegistResponse> > stream(stub->regist(&context));
		stream->Write(registRequest);
		stream->WritesDone();
		Status status = stream->Finish();
	    if (!status.ok()) {
	      std::cout << "regist rpc failed." << std::endl;
	    }
	    else
	    {
	    	cout << "regist rpc success." << std::endl;
	    }

	    RegistResponse registResponse;
		while (stream->Read(&registResponse))
		{
			cout << "get registResponse: " << registResponse.basic().code() << " " << registResponse.basic().msg() << endl;
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
		registRequest.set_user_name(userNameEncrypted);
		registRequest.set_pass_word(passWordEncrypted);
		return 0;
	}

};




#endif