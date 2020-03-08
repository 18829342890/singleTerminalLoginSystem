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
#include "commandLine.h"
#include "Help.h"
#include "logrecord.h"
#include "myTool.h"
#include "receiveLogoutNotice.h"
#include "messageReceiver.grpc.pb.h"

using namespace std;
using namespace client::service;
using namespace client::tool;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using proto::messageReceiver::messageReceiver;
using proto::messageReceiver::SyncClientInfoRequest;
using proto::messageReceiver::SyncClientInfoResponse;

const int MAX_MESSAGE_SIZE = 10240;
const char* DST_IP_PORT = "localhost:50051";
const char* CLIENT_IP_PORT = "127.0.0.1:8081";

static int s_clientLocalPort = 50052;
static string s_clientLocalIp = "localhost";
int isLogined = 0;
string userName;
int isNoticeLogout = 0;



//作为服务端，接收服务器推送的信息
static void* receiveMessage(void* param)
{
	//获取本地ip和可用的端口
	//if(getLocalIP("eth0", s_clientLocalIp) != 0);
	//if(getAvailablePort(s_clientLocalPort) != 0) exit(1);

	std::stringstream address;
	address << s_clientLocalIp << ":" << s_clientLocalPort;

	//创建grpc
	receiveLogoutNotice service;
	std::string key = myTool::readFile2String("../../auth/client.key");
  	std::string cert = myTool::readFile2String("../../auth/client.crt");
  	std::string root = myTool::readFile2String("../../auth/ca.crt");
  	grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert = {key,cert};
  	grpc::SslServerCredentialsOptions sslOps;
  	sslOps.pem_root_certs = root;
  	sslOps.pem_key_cert_pairs.push_back (key_cert);

	ServerBuilder builder;
	builder.AddListeningPort(address.str().c_str(), grpc::SslServerCredentials(sslOps));
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	LOG_INFO("Server listening on %s", address.str().c_str());

  	server->Wait();
}

//同步客户端ip和port
static void* syncClientInfo(void* param)
{
	auto cacert = myTool::readFile2String("../../auth/ca.crt");
  	auto options = grpc::SslCredentialsOptions();
  	options.pem_root_certs = cacert;
  	auto creds = grpc::SslCredentials(options);

	std::shared_ptr<Channel> channel = grpc::CreateChannel(DST_IP_PORT, creds);
	std::shared_ptr<messageReceiver::Stub> stub = messageReceiver::NewStub(channel);
	if(!stub)
	{
		printf("connect to service failed!\n");
		return NULL;
	}

	while(true)
	{
		if(!isLogined) continue;

		//登录之后，同步客户端信息
		SyncClientInfoRequest syncClientInfoRequest;
		syncClientInfoRequest.set_user(userName);
		syncClientInfoRequest.set_ip(s_clientLocalIp);
		syncClientInfoRequest.set_port(s_clientLocalPort);

		ClientContext context;
		SyncClientInfoResponse syncClientInfoResponse;
		context.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
		Status status = stub->syncClientInfo(&context, syncClientInfoRequest, &syncClientInfoResponse);
		if(status.ok())
		{
			LOG_INFO("%s", syncClientInfoResponse.message().c_str());
		}
		else
		{
			LOG_INFO("syncClientInfo failed!");
		}

		sleep (5);
	}

	return NULL;
}

//监控服务端是否发送退出登录请求
static void* monitorIsNoticeLogout(void* params)
{
	while(true)
	{
		if(isNoticeLogout) exit(0);
		sleep(1);
	}
}

//处理ctrl + c 信号
static void sigintHandler(int sig)
{
	printf("please input exit to exit!\n");
}

int main()
{
	//防止ctrl + c 退出，需执行exit
	signal(SIGINT, sigintHandler);

	//作为客户端，连接服务器
	char cmds[MAX_CMD_LEN] = {0};
	auto cacert = myTool::readFile2String("../../auth/ca.crt");
  	auto options = grpc::SslCredentialsOptions();
  	options.pem_root_certs = cacert;
  	auto creds = grpc::SslCredentials(options);

	std::shared_ptr<Channel> channel = grpc::CreateChannel(DST_IP_PORT, creds);
	std::shared_ptr<messageReceiver::Stub> stub = messageReceiver::NewStub(channel);
	if(!stub)
	{
		printf("connect to service failed!\n");
		return -1;
	}

	//创建一个线程，作为服务器接收客户端信息
	pthread_t serverTid;
	if(pthread_create(&serverTid, NULL, receiveMessage, NULL) < 0)
	{
		printf("create pthread failed!\n");
		return -1;
	}

	//创建一个线程定时同步客户端信息
	pthread_t syncClientInfoTid;
	if(pthread_create(&syncClientInfoTid, NULL, syncClientInfo, NULL) < 0)
	{
		printf("create pthread failed!\n");
		return -1;
	}

	//创建一个线程监控是否通知退出
	pthread_t monitorIsNoticeLogoutTip;
	if(pthread_create(&syncClientInfoTid, NULL, monitorIsNoticeLogout, NULL) < 0)
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
			help.processCommandLine(stub, params);
			continue;
		}

		//执行命令
		processCmd(stub, cmd, params);
	}

	pthread_join(serverTid, NULL);
	pthread_join(syncClientInfoTid, NULL);
	pthread_join(monitorIsNoticeLogoutTip, NULL);
	return 0;
}