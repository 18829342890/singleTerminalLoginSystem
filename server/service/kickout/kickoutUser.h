#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"



class KickoutUser : public LoginManageServiceBase
{
public:
	KickoutUser(const sql::Connection* mysqlConnect);
	virtual ~KickoutUser();
	int processkickout(const string& userName);
};