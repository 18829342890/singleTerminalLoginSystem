#ifndef __MY_LOGOUT_H__
#define __MY_LOGOUT_H__

#include "ProcessCommandLineBase.h"
#include <stdlib.h>


class LogoutCmd : public ProcessCommandLineBase
{

public:

	int processCommandLine(std::shared_ptr<messageReceiver::Stub> _stub, const char* params[])
	{
		// //发送退出登录通知
		// string message;
		// int ret = getJsonStringFromParams(params, message);
		// if(ret < 0)
		// {
		// 	return -1;
		// }

		// ret = write(clientSocket, message.c_str(), message.length());
		// if(ret < 0)
		// {
		// 	printf("write failed! ERROR: %s\n", strerror(errno));
		// 	return -1;
		// }

		// char buf[MESSAGE_MAX_LEN] = {0};
		// ret = read(clientSocket, buf, sizeof(buf));
		// if(ret < 0)
		// {
		// 	return -1;
		// }

		// buf[ret] = '\0';
		// printf("%s\n", buf);
		
		return 0;
	}

	int getJsonStringFromParams(const char* params[], string& message)
	{
		// //判断参数是否合法
		// int i = 0;
		// for(; params[i]; ++i){}

		// if(i != 2)
		// {
		// 	cout << "please input regist username password!" << endl;
		// 	return -1;
		// }

		// //设置username、password
		// registRequest.set_user_name(params[0]);
		// registRequest.set_pass_word(params[1]);
		// return 0;
		// return 0;
	}

};





#endif