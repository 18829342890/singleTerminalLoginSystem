#pragma once

#include <string>
#include <vector>
#include <hiredis/hiredis.h>
#include "server/domain/userLoginCacheBO.h"


namespace userLoginService{
namespace infrastructure{

using namespace std;
using namespace userLoginService::BO;

class UserLoginCache
{

public:
	UserLoginCache(const redisContext* redisConnect);
	~UserLoginCache();

	int getUserLoginCacheByUserName(const string& userName, UserLoginCacheBO& userLoginCache);
	void cacheUserLoginInfo(UserLoginCacheBO userLoginCache, int ttl);
	void updateCacheTtl(const string& key, int ttl);




private:
	//执行一条命令
	inline void justExecuteCmd(const string& cmd);
	//事务执行多条命令
	void executeCmds(const vector<string>& cmds);
	
	inline uint64_t strToUint64(const string& str);
	inline uint64_t strToLongInt(const string& str);
	template <class T> inline string type2str(T v);
	int getIntValueByCmd(const string& cmd, int& value);
	int getUint64ValueByCmd(const string& cmd, uint64_t& value);
	int getStringValueByCmd(const string& cmd, string& value);




private:
    const redisContext* _redisConnect;
};

}
}