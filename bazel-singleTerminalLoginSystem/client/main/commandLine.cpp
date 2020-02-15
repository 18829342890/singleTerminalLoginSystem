#include "commandLine.h"
#include "Help.h"
#include <string.h>


#define GET_CMD_IMPL_CLASS (cmdImplClass)  \
do{                                        \
	IProcessCommandLine implClass; \
}while(0)



bool isLegalCmd(const char* cmd)
{
	for(int i = 0; s_my_cmds[i].cmd; ++i)
	{
		if(strcmp(cmd, s_my_cmds[i].cmd) == 0)
		{
			return true;
		}
	}

	return false;
}

ProcessCommandLineBase* getCmdImplClass(const char* cmd)
{
	for(int i = 0; s_my_cmds[i].cmd; ++i)
	{
		if(strcmp(cmd, s_my_cmds[i].cmd) == 0)
		{
			return s_my_cmds[i].cmdImplClass;
		}
	}

	return NULL;
}

const char* getCmdImplDesc(const char* cmd)
{
	for(int i = 0; s_my_cmds[i].cmd; ++i)
	{
		if(strcmp(cmd, s_my_cmds[i].cmd) == 0)
		{
			return s_my_cmds[i].cmdDesc;
		}
	}

	return NULL;
}

int processCmd(const char* cmd, const char* params[])
{
	ProcessCommandLineBase* cmdImplClass = getCmdImplClass(cmd);
	if(cmdImplClass == NULL)
	{
		Help help;
		help.processCommandLine(params);
		return -1;
	}

	return cmdImplClass->processCommandLine(params);
}