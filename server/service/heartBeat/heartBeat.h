#pragma once

#include<string>
#include "server/service/base/loginManageServiceBase.h"

using namespace std;
using namespace userLoginService::BO;

class HeartBeat : public LoginManageServiceBase
{

public:
	HeartBeat(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoTtl);
	virtual ~HeartBeat();

	int processHeartBeat(const string& userName, const string& clientUid, int& clientOperation);
	void updateUserLoginInfoCacheTtl(const string& userName);

private:
	int _userLoginInfoTtl;

};