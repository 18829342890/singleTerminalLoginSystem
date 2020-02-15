#ifndef __I_PROCESS_COMMAND_LENE_H__
#define __I_PROCESS_COMMAND_LENE_H__

#include <stdio.h>
#include <string>
#include <sstream>
#include <unistd.h>
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

	//string getJsonStringFromParams(const char* params[]);

};

// //lib问题 先不用json
// string ProcessCommandLineBase::getJsonStringFromParams(const char* params[])
// {
// 	std::stringstream s;
// 	for(int i = 0; i < sizeof(params); ++i)
// 	{
// 		s << params[i];
// 		if(i != sizeof(params) - 1)
// 		{
// 			s << ",";
// 		}
// 	}

// 	printf("s:%s\n", s.str().c_str());
// 	return s.str();
// }



#endif