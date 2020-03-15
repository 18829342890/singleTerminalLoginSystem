#pragma once

#include <string>


namespace userLoginService{
namespace BO{

using namespace std;

class UserLoginCacheBO
{

public:
	string userName;
	uint64_t clientUid;
	int status;


	void setUserName(string userName)
	{
		this->userName = userName;
	}

	void setClientUid(uint64_t clientUid)
	{
		this->clientUid = clientUid;
	}

	void setStatus(uint64_t status)
	{
		this->status = status;
	}

	string getUserName()
	{
		return this->userName;
	}

	uint64_t getClientUid()
	{
		return this->clientUid;
	}

	int getStatus()
	{
		return this->status;
	}

};



}
}