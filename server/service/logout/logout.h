#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"



class Logout : public LoginManageServiceBase
{
public:
	Logout(const sql::Connection* mysqlConnect);
	virtual ~Logout();

	int processLogout(const string& userName, const string& clientUid);
};




