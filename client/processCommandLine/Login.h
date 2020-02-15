#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"


class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(int clientSocket, const char* params[])
	{
		string message = getJsonStringFromParams(params);
		int ret = write(clientSocket, message, message.length());
		if(ret < 0)
		{
			printf("write failed! ERROR: %s\n", strerror(errno));
			return -1;
		}

		char buf[MESSAGE_MAX_LEN] = {0};
		ret = read(clientSocket, buf, sizeof(buf));
		if(ret < 0)
		{
			printf("read failed! ERROR: %s\n", strerror(errno));
			return -1;
		}

		buf[ret] = '\0';
		printf("%s\n", buf);

		return 0;
	}


	//lib问题 先不用json
	string getJsonStringFromParams(const char* params[])
	{
		std::stringstream s;
		for(int i = 0; i < sizeof(params); ++i)
		{
			s << params[i];
			if(i != sizeof(params) - 1)
			{
				s << ",";
			}
		}

		printf("s:%s\n", s.str().c_str());
		return s.str();
	}
}




#endif