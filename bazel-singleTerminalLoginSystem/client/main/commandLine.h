#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__
#include <stdbool.h>
#include <stdio.h>
#include "ProcessCommandLineBase.h"
#include "Help.h"

typedef struct 
{
	const char* cmd;           //命令
	ProcessCommandLineBase* cmdImplClass;  //命令实现类
	const char* cmdDesc;       //命令描述
}my_cmd_t;

const int MAX_CMD_LEN = 128;



static Help* help = new Help();
static my_cmd_t s_my_cmds[] = {
	{
		"help",
		help,
		"help user use cmd."
	},
	{
		"?",
		help,
		"help user use cmd."
	},
	{
		NULL,
		NULL,
		NULL,
	}
};


//判断命令是否合法
bool isLegalCmd(const char* cmd);

//获取cmd的实现类
ProcessCommandLineBase* getCmdImplClass(const char* cmd);

//获取cmd的描述
const char* getCmdImplDesc(const char* cmd);

//执行命令
int processCmd(const char* cmd, const char* params[]);




















#endif