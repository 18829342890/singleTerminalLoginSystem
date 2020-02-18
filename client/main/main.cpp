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
const int PORT = 8080;
const char* IP = "127.0.0.1";

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

//设置套接字为非阻塞
static int setNonBlocking(int fd )
{
    int oldOption = fcntl( fd, F_GETFL );
    int newOption = oldOption | O_NONBLOCK;
    fcntl( fd, F_SETFL, newOption );
    return oldOption;
}

//获取服务端连接
static int getTcpSocket()
{
	int32_t ret = 0;
    int skfd = -1;
    struct sockaddr_in server;

    //创建套接字
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if (skfd < 0)
    {
        printf("socket create failed! ERROR: %s\n", strerror(errno));
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    ret = connect(skfd, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0)
    {
        printf("connect failed! ERROR: %s\n", strerror(errno));
        close(skfd);
        return -1;
    }

    return skfd;
}

//单独接收服务器信息的线程
static void* justRecvMessage(void* param)
{
	char message[MAX_MESSAGE_SIZE] = {0};

	while(1)
	{
		message[0] = '\0';
		int ret = read(clientSocket, message, sizeof(message));
		message[ret] = '\0';

		if(strlen(message) > 0)
		{
			printf("%s\n", message);
			if(strcmp(message, "logout") == 0)
			{
				printf("您的账号在另一台设备登录，当前设备退出登录!\n");
				exit(0);
			}
		}

		usleep(50);
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

    //创建套接字
	char cmds[MAX_CMD_LEN] = {0};
	clientSocket = getTcpSocket();
	if(clientSocket < 0)
	{
		return -1;
	}

	setNonBlocking(clientSocket);

	//创建一个线程专门接收服务器主动发送的信息
	pthread_t tid;
	if(pthread_create(&tid, NULL, justRecvMessage, NULL) < 0)
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
			help.processCommandLine(clientSocket, params);
			continue;
		}

		//执行命令
		processCmd(clientSocket, cmd, params);
	}

	pthread_join(tid, NULL);
}