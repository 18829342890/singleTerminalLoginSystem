#ifndef __HELP_H__
#define __HELP_H__

#include "ProcessCommandLineBase.h"

using namespace client::commandLine;

class Help : public ProcessCommandLineBase
{

public:

	virtual int processCommandLine(std::shared_ptr<userLoginManageService::Stub> stub, const string& clientUuid, const char* params[])
	{
		processHelp();
		return 0;
	}

private:
	void processHelp()
	{
		printf("###############################################################################################################\n");
		printf("######### help/? : Show all legal commands                                        #############################\n");
		printf("######### exit   : logout then exit client.                                       #############################\n");
		printf("######### regist : user regist. for example: regist username password             #############################\n");
		printf("######### login  : login server.for example: login username password              #############################\n");
		printf("######### logout : logout.      for example: logout                               #############################\n");
		printf("######### kickout: kickout user.for example: kickout username                     #############################\n");
		printf("###############################################################################################################\n");
	}

};





#endif