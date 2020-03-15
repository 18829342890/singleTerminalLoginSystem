#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

#include <stdbool.h>
#include <stdio.h>
#include "ProcessCommandLineBase.h"
#include "Help.h"
#include "Login.h"
#include "Exiter.h"
#include "Regist.h"
#include "Logout.h"


const int MAX_CMD_LEN = 128;

typedef struct 
{
	const char* cmd;           //命令
	ProcessCommandLineBase& cmdImplClass;  //命令实现类
	const char* cmdDesc;       //命令描述
}my_cmd_t;


static Help help;
static Login login;
static LogoutCmd logoutCmd;
static Exiter exiter;
static RegistCmd registCmd;
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
		"login",
		login,
		"login server."
	},
	{
		"logout",
		logoutCmd,
		"logout user."
	},
	{
		"exit",
		exiter,
		"logout then exit client."
	},
	{
		"regist",
		registCmd,
		"user regist."
	},
	{
		NULL,
		help,
		NULL,
	}
};


//判断命令是否合法
bool isLegalCmd(const char* cmd);

//获取cmd的描述
const char* getCmdImplDesc(const char* cmd);

//执行命令
int processCmd(std::shared_ptr<userLoginManageService::Stub> stub, const char* cmd, const char* params[]);









#endif