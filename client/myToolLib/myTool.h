#pragma once

namespace client{
namespace tool{

class myTool
{
public:
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
	}

	static std::string readFile2String(std::string file_name) {
		std::ifstream ifs(file_name);
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		return buffer.str();
	}


	//获取本地ip  ios系统不行！！！
	static int getLocalIP(const std::string& strIfName,std::string& strIPAddr)
	{
	    int sock_get_ip = socket(AF_INET, SOCK_STREAM, 0);
	    if (sock_get_ip < 0)
	    {
	    	printf("socket system call failed! errno:%d, errmsg:%s", errno, strerror(errno));
	        return -1;
	    }

	    struct ifreq ifr_ip;
	    memset(&ifr_ip, 0, sizeof(ifr_ip));
	    strncpy(ifr_ip.ifr_name, strIfName.c_str(), sizeof(ifr_ip.ifr_name) - 1);

	    if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )
	    {
	    	//printf("ioctl system call failed! errno:%d, errmsg:%s\n", errno, strerror(errno));
	        close(sock_get_ip);
	        return -2;
	    }

	    struct sockaddr_in *sin;
	    sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;

	    char ipaddr[16] = { 0 };
	    strcpy(ipaddr,inet_ntoa(sin->sin_addr));
	    close(sock_get_ip);

	    strIPAddr = ipaddr;
	    return 0;
	}

	//获取可用的port
	static int getAvailablePort(int& port)
	{
	    int sock = socket(AF_INET, SOCK_STREAM, 0);
	    if (sock < 0) 
	    {
	    	printf("socket system call failed! errno:%d, errmsg:%s\n", errno, strerror(errno));
	    	return -1;
	    }

	    struct sockaddr_in addr;
	    addr.sin_family = AF_INET;
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    addr.sin_port = 0;// 若port指定为0,则调用bind时，系统会为其指定一个可用的端口号
	    int ret = ::bind(sock, (struct sockaddr *) &addr, sizeof(sockaddr_in));
	    if(ret != 0)
	    {
	    	printf("bind system call failed! errno:%d, errmsg:%s\n", errno, strerror(errno));
	    	close(sock);
	    	return -1;
	    }

	    struct sockaddr_in sockaddr;
	    int len = sizeof(sockaddr);
	    ret = getsockname(sock, (struct sockaddr *) &sockaddr, (socklen_t *) &len);
	    if(ret != 0)
	    {
	    	printf("getsockname system call failed! errno:%d, errmsg:%s\n", errno, strerror(errno));
	    	close(sock);
	    	return -1;
	    }

	    port = ntohs(sockaddr.sin_port);
	    close(sock);
	    return 0;
	}

};


}
}