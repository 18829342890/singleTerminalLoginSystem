#pragma once
#include <string>

namespace userLoginService{
namespace BO{

using namespace std;

class UserLoginManageBO
{
public:
	uint64_t id;
	string   userName;
	string   ip;
	int16_t  port;
	int8_t   status;
	uint64_t createTime;
	//updateTime 由mysql触发器管理


	//set
	void setId(uint64_t id)
	{
		this->id = id;
	}
	
	void setUserName(const string& userName)
	{
		this->userName = userName;
	}

	void setIp(const string& ip)
	{
		this->ip = ip;
	}

	void setPort(int16_t port)
	{
		this->port = port;
	}

	void setStatus(int8_t status)
	{
		this->status = status;
	}

	void setCreateTime(uint64_t createTime)
	{
		this->createTime = createTime;
	}

	//get
	uint64_t getId() 
	{
		return this->id;
	}

	string getUserName() 
	{
		return this->userName;
	}

	string getIp()
	{
		return this->ip;
	}

	int16_t getPort() 
	{
		return this->port;
	}

	int8_t getStatus()
	{
		return this->status;
	}

	uint64_t getCreateTime()
	{
		return this->createTime;
	}
};


}
}