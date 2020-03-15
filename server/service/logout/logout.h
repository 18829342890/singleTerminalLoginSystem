#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"



class Logout : public LoginManageServiceBase
{
public:
	Logout(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoCacheTtl);
	virtual ~Logout();

	int processLogout(const string& userName, uint64_t clientUid);
	void cacheUserLogoutInfo(const string& userName, uint64_t clientUid);

private:
	SqlApi _sqlApi;
	int _userLoginInfoCacheTtl;
};




