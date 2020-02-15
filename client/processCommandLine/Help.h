#ifndef __HELP_H__
#define __HELP_H__

#include "ProcessCommandLineBase.h"


class Help : public ProcessCommandLineBase
{

public:

	virtual int processCommandLine(const char* params[])
	{
		processHelp();
		return 0;
	}

private:
	void processHelp()
	{
		printf("########################################################################\n");
		printf("######### help/? : Show all legal commands #############################\n");
		printf("########################################################################\n");
	}

};





#endif