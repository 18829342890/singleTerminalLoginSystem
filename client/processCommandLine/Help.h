#ifndef __HELP_H__
#define __HELP_H__

#include "ProcessCommandLineBase.h"


class Help : public ProcessCommandLineBase
{

public:

	virtual int processCommandLine(int clientSocket, const char* params[])
	{
		processHelp();
		return 0;
	}

private:
	void processHelp()
	{
		printf("###############################################################################################################\n");
		printf("######### help/? : Show all legal commands                                        #############################\n");
		printf("######### exit   : logout then exit client. for example:exit username             #############################\n");
		printf("######### regist : user regist. for example: regist username password             #############################\n");
		printf("######### login  : login server.for example: login username password              #############################\n");
		printf("######### logout : logout user. for example: logout username                      #############################\n");
		printf("###############################################################################################################\n");
	}

};





#endif