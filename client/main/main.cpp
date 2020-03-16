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
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using proto::userLoginManage::userLoginManageService;
using proto::userLoginManage::HeartBeatRequest;
using proto::userLoginManage::HeartBeatResponse;

static string s_serverAddress;   //服务器的地址
static int s_heartBeatInterval;  //发送心跳包的时间间隔
static string s_clientUuid;        //客户端uuid

int isLogined = 0;               //是否登录的标志
string userName;                 //登录之后的用户名


//同步客户端ip和port
static void* heartBeat(void* param)
{	
	bool isEncoded = false;
	char userNameEncrypted[1024] = {0};
	auto stub = static_cast<std::shared_ptr<userLoginManageService::Stub>*>(param);

	while(true)
	{
		if(isLogined)
		{
			//编码用户名
			if(!isEncoded)
			{
				isEncoded = true;
				base64_encode(userName.c_str(), userName.length(), userNameEncrypted);
			}
			
			//发送心跳请求
			ClientContext context;
			HeartBeatRequest heartBeatRequest;
			std::shared_ptr<ClientReaderWriter<HeartBeatRequest, HeartBeatResponse> > stream((*stub)->heartBeat(&context));

			heartBeatRequest.mutable_basic()->set_uuid(s_clientUuid);
			heartBeatRequest.set_user_name(userNameEncrypted);
			stream->Write(heartBeatRequest);
			stream->WritesDone();

			//获取响应
			HeartBeatResponse heartBeatResponse;
			stream->Read(&heartBeatResponse);
			if(heartBeatResponse.basic().code() != SUCCESS)
			{
				LOG_ERROR("heartBeatResponse code is not SUCCESS! errmsg:%s", heartBeatResponse.basic().msg().c_str());
			}
			else
			{
				if(heartBeatResponse.operation() == OTHER_DEVICE_LOGINED_JUST_LOGOUT)
				{
					cout << "您的账号已在另一台设备上登录，退出当前登录!" << endl;
					exit(0);
				}
				else if(heartBeatResponse.operation() == KICKOUT_BY_MANAGER_JUST_LOGOUT)
				{
					cout << "您已被管理员踢出登录，请再次登录!" << endl;
					isLogined = 0;
					userName = "";
				}
			}
		}

		usleep(s_heartBeatInterval);
	}
	return NULL;
}

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

int main()
{
	//防止ctrl + c 退出，需执行exit
	signal(SIGINT, sigintHandler);

	//生成客户端uuid
	uuid_t uuid;
	char uuidStr[36];
	uuid_generate(uuid);
	uuid_unparse(uuid, uuidStr);
	s_clientUuid = uuidStr;

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

	//创建一个线程定时发送心跳包请求
	pthread_t heartBeatTid;
	if(pthread_create(&heartBeatTid, NULL, heartBeat, &stub) < 0)
	{
		printf("create pthread failed!\n");
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
		processCmd(stub, s_clientUuid, cmd, params);
	}

	pthread_join(heartBeatTid, NULL);
	return 0;
}