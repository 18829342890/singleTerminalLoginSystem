#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"
#include "Enum.h"


class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(std::shared_ptr<messageReceiver::Stub> stub, const char* params[])
	{
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


private:
	int getJsonStringFromParams(const char* params[], string& message)
	{
		//判断是否合法
		int i = 0;
		for(; params[i]; ++i){}

		if(i != 2)
		{
			printf("please input login username password!\n");
			return -1;
		}

		const char* userName = params[0];
		const char* passWord = params[1];

		// //创建json
		// cJSON* root = cJSON_CreateObject();

		// //messageTyep
		// cJSON* messageType = cJSON_CreateNumber(LOGIN);
		// cJSON_AddItemToObject(root, "messageType", messageType);

		// //message
		// cJSON* messageItem = cJSON_CreateObject();
		// cJSON* userNameItem = cJSON_CreateString(userName);
		// cJSON* passWordItem = cJSON_CreateString(passWord);
		// cJSON_AddItemToObject(messageItem, "userName", userNameItem);
		// cJSON_AddItemToObject(messageItem, "passWord", passWordItem);
		// cJSON_AddItemToObject(root, "message", messageItem);

		// //转化为string
		// std::stringstream ss;
		// ss << cJSON_PrintUnformatted(root);
		// message = ss.str();

		// cJSON_Delete(root);
		// return 0;
	}

};




#endif