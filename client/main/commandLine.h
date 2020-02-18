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

typedef struct 
{
	const char* cmd;           //命令
	ProcessCommandLineBase* cmdImplClass;  //命令实现类
	const char* cmdDesc;       //命令描述
}my_cmd_t;

const int MAX_CMD_LEN = 128;



static Help* help = new Help();
static Login* login = new Login();
static LogoutCmd* logoutCmd = new LogoutCmd();
static Exiter* exiter = new Exiter();
static RegistCmd* registCmd = new RegistCmd();
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
int processCmd(int clientSocket, const char* cmd, const char* params[]);




















#endif