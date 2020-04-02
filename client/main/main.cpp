#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fstream>
#include <sstream>
#include <grpcpp/grpcpp.h>
#include <uuid/uuid.h>

#include "client/commanLine/commandLine.h"
#include "client/commanLine/processCommandLine/Help.h"
#include "mylib/mylibLog/logrecord.h"
#include "mylib/myLibIni/simpleIni.h"
#include "client/myToolLib/myTool.h"
#include "protos/proto/userLoginManage.grpc.pb.h"

using namespace std;
using namespace client::tool;

static string s_serverAddress;   //服务器的地址
static int s_heartBeatInterval;  //发送心跳包的时间间隔
static string s_clientUuid;        //客户端uuid

int isLogined;
string userName;

//处理ctrl + c 信号
static void sigintHandler(int sig)
{
	printf("please input exit to exit!\n");
}

static void ininConfig()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("../config/client_config.ini");

	s_serverAddress = ini.GetValue("server", "address", "localhost:50051");
	s_heartBeatInterval = strtol(ini.GetValue("client", "heartBeatInterval", "1000000"), NULL, 10);

	LOG_INFO("s_serverAddress:%s, s_heartBeatInterval:%d", s_serverAddress.c_str(), s_heartBeatInterval);
}

static void genClientUuid()
{
	uuid_t uuid;
	char uuidStr[36];
	uuid_generate(uuid);
	uuid_unparse(uuid, uuidStr);
	s_clientUuid = uuidStr;
}

int main()
{
	//防止ctrl + c 退出，需执行exit
	signal(SIGINT, sigintHandler);

	//生成客户端uuid
	genClientUuid();

	//初始化配置
	ininConfig();

	//连接服务器
	char cmds[MAX_CMD_LEN] = {0};
	auto cacert = myTool::readFile2String("../../auth/ca.crt");
  	auto options = grpc::SslCredentialsOptions();
  	options.pem_root_certs = cacert;
  	auto creds = grpc::SslCredentials(options);

	std::shared_ptr<Channel> channel = grpc::CreateChannel(s_serverAddress, creds);
	std::shared_ptr<userLoginManageService::Stub> stub = userLoginManageService::NewStub(channel);
	if(!stub)
	{
		printf("connect to service failed!\n");
		return -1;
	}

	while(1)
	{
		//获取输入
		cmds[0] = '\0';
		fgets(cmds, MAX_CMD_LEN, stdin);
		cmds[strlen(cmds) - 1] = '\0';
		if(strlen(cmds) <= 0)
		{
			continue;
		}
		
		//获取命令和参数
		std::string cmdParams = cmds;
		std::vector<std::string> cmd_params_v;
		myTool::str2vec(cmdParams, cmd_params_v, " ");

		const char* cmd = cmd_params_v[0].c_str();
		const char* params[10] = {NULL};
		for(int i = 1; i < cmd_params_v.size(); ++i)
		{
			params[i-1] = cmd_params_v[i].c_str();
		}

		//判断命令是否合法
		if( !isLegalCmd(cmd) )
		{
			Help help;
			printf("command error! Please refer to the following introduction:\n");
			help.processCommandLine(stub, s_clientUuid, params);
			continue;
		}

		//执行命令
		std::thread cmdThread(processCmd, stub, s_clientUuid, cmd, params);
		cmdThread.detach();
	}

	return 0;
}