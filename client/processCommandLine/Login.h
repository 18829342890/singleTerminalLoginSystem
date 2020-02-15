#ifndef __LOGIN_H__
#define __LOGIN_H__


#include "ProcessCommandLineBase.h"


class Login : public ProcessCommandLineBase
{
public:
	virtual int processCommandLine(int clientSocket, const char* params[])
	{
		int ret = write(clientSocket, params[0], strlen(params[0]));
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
}




#endif