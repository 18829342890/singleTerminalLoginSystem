#pragma once
#include <string>

namespace userLoginService{
namespace BO{

using namespace std;

class UserPassWordBO
{
public:
	uint64_t id;
	string   userName;
	string   passWord;
	string   salt;
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

	void setPassWord(const string& passWord)
	{
		this->passWord = passWord;
	}

	void setSalt(const string& salt)
	{
		this->salt = salt;
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

	string getPassWord() 
	{
		return this->passWord;
	}

	string getSalt() 
	{
		return this->salt;
	}

	uint64_t getCreateTime() 
	{
		return this->createTime;
	}
};




}
}



