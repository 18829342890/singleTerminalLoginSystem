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
	string clientUid;
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

	void setClientUid(string clientUid)
	{
		this->clientUid = clientUid;
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

	string getClientUid()
	{
		return this->clientUid;
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