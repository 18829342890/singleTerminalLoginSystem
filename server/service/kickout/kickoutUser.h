#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"



class KickoutUser : public LoginManageServiceBase
{
public:
	KickoutUser(const sql::Connection* mysqlConnect, const redisContext* redisConnect, int userLoginInfoCacheTtl);
	virtual ~KickoutUser();
	int processkickout(const string& userName);
	void cacheUserLogoutInfo(const string& userName);

private:
	int _userLoginInfoCacheTtl;
};