#ifndef __MY_EXIT_H__
#define __MY_EXIT_H__

#include "ProcessCommandLineBase.h"
#include <stdlib.h>


class Exiter : public ProcessCommandLineBase
{

public:

	int processCommandLine(int clientSocket, const char* params[])
	{
		//TODO 发送退出登录
		exit(0);
		return 0;
	}

};





#endif