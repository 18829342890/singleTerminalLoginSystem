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
#include "commandLine.h"
#include "Help.h"
#include "logrecord.h"

using namespace std;

const int PORT = 8080;
const char* IP = "127.0.0.1";


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

static int getTcpSocket()
{
	int32_t ret = 0;
    int skfd = -1;
    struct sockaddr_in server;

    //创建套接字
    skfd = socket(AF_LOCAL, SOCK_STREAM, 0);
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


int main()
{
	char cmds[MAX_CMD_LEN] = {0};
	int clientSocket = getTcpSocket();
	if(clientSocket < 0)
	{
		return -1;
	}

	while(1)
	{
		//获取输入
		cmds[0] = '\0';
		fgets(cmds, MAX_CMD_LEN, stdin);
		cmds[strlen(cmds) - 1] = '\0';
		
		//获取命令和参数
		std::string cmdParams = cmds;
		std::vector<std::string> cmd_params_v;
		str2vec(cmdParams, cmd_params_v, " ");

		const char* cmd = cmd_params_v[0].c_str();
		const char* params[10] = {NULL};
		for(int i = 1; i < cmd_params_v.size(); ++i)
		{
			params[i] = cmd_params_v[i].c_str();
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
}