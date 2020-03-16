#pragma once

#include <string>


namespace userLoginService{
namespace BO{

using namespace std;

class UserLoginCacheBO
{

public:
	string userName;
	string clientUid;
	int status;


	void setUserName(string userName)
	{
		this->userName = userName;
	}

	void setClientUid(string clientUid)
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

	string getClientUid()
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