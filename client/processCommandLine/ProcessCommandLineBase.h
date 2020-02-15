#ifndef __I_PROCESS_COMMAND_LENE_H__
#define __I_PROCESS_COMMAND_LENE_H__

#include <stdio.h>

const int MESSAGE_MAX_LEN = 10240;

class ProcessCommandLineBase
{

public:
	ProcessCommandLineBase(){};
	~ProcessCommandLineBase(){};

	/*
	 * 处理命令的公共接口
	 */
	virtual int processCommandLine(int clientSocket, const char* params[]){}
};




#endif