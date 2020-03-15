#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"



class KickoutUser : public LoginManageServiceBase
{
public:
	KickoutUser(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoCacheTtl);
	virtual ~KickoutUser();
	int processkickout(const string& userName);
	void cacheUserLogoutInfo(const string& userName);

private:
	SqlApi _sqlApi;
	int _userLoginInfoCacheTtl;
};