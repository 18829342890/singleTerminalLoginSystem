#ifndef __SYNC_CLIENT_INFO_H__
#define __SYNC_CLIENT_INFO_H__

#include "mylib/mylibSql/sqlApi.h"
#include <string>



class SyncClientInfo
{
public:
	SyncClientInfo(SqlApi& sqlApi);
	virtual ~SyncClientInfo();
	int processSyncClientInfo(const string& userName, const string& ip, int port, int& code, string& msg);

private:
	//该用户是否已登录
	int getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined);
	int updateUserClientInfo(const string& userName, const string& ip, int port);

private:
	SqlApi _sqlApi;
};






#endif