#pragma once

#include<string>
#include "server/domain/userLoginCacheBO.h"
#include "server/service/base/loginManageServiceBase.h"

using namespace std;
using namespace userLoginService::BO;

class HeartBeat : public LoginManageServiceBase
{

public:
	HeartBeat(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoTtl);
	virtual ~HeartBeat();

	int processHeartBeat(const string& userName, uint64_t clientUid, int& clientOperation);
	void updateUserLoginInfoCacheTtl(const string& userName);

private:
	int getUserLoginInfo(const string& userName, UserLoginCacheBO& userLoginCache);

private:
	int _userLoginInfoTtl;

};