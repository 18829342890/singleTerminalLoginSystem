#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
#include "commandLine.h"
#include "Help.h"
#include "logrecord.h"

using namespace std;

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


int main()
{
	char cmds[MAX_CMD_LEN] = {0};

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
			help.processCommandLine(params);
			continue;
		}

		//执行命令
		processCmd(cmd, params);





	}
}