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
		printf("########################################################################\n");
		printf("######### help/? : Show all legal commands #############################\n");
		printf("######### exit   : exit client             #############################\n");
		printf("########################################################################\n");
	}

};





#endif