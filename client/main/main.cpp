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
#include "commandLine.h"
#include "Help.h"

using namespace std;

const int MAX_MESSAGE_SIZE = 10240;
const char* DST_IP_PORT = "127.0.0.1:8080";

static int clientSocket = 0;

//string to vector
static void str2vec(const std::string& src, std::vector<std::string>& vec,
                        const std::string seg = ",") 
{
    if(src.empty()) { return; }

    std::string str_tmp;
    size_t pos_start = 0, pos_end = 0;

    do {
        pos_end = src.find(seg, pos_start);
        if (pos_end == std::string::npos) {
            str_tmp = src.substr(pos_start);
        } else {
            str_tmp = src.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + seg.length();
        }

        vec.push_back(str_tmp);
    }while(pos_end != std::string::npos);

    return;
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

	//创建rpc连接
	char cmds[MAX_CMD_LEN] = {0};
	ChannelArguments channelArgs;

	channelArgs.SetCompressionAlgorithm(GRPC_COMPRESS_GZIP);
	std::shared_ptr<Channel> channel = grpc::CreateCustomChannel(DST_IP_PORT, grpc::InsecureChannelCredentials(), channelArgs);
	std::shared_ptr<messageReceiver::Stub> stub = messageReceiver::NewStub(channel);
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
		str2vec(cmdParams, cmd_params_v, " ");

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
}