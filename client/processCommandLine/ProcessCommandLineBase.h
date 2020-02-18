#ifndef __I_PROCESS_COMMAND_LENE_H__
#define __I_PROCESS_COMMAND_LENE_H__

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <errno.h>
// #include "mylib/jsoncpp/include/json.h"
// #include "mylib/jsoncpp/include/writer.h"

using namespace std;

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