#ifndef __SYNC_CLIENT_INFO_H__
#define __SYNC_CLIENT_INFO_H__

#include "mylog/mylibSql/sqlApi.h"
#include <string>



class SyncClientInfo
{
public:
	SyncClientInfo(SqlApi& sqlApi);
	virtual ~SyncClientInfo();
	int processSyncClientInfo(const string& userName, const string& ip, int port, int& code, string& msg);

private:
	SqlApi _sqlApi;
};






#endif