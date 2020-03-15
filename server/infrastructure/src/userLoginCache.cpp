#include <sstream>
#include <hiredis/hiredis.h>
#include "mylib/mylibLog/logrecord.h"
#include "server/infrastructure/include/userLoginCache.h"


using namespace userLoginService::infrastructure;


UserLoginCache::UserLoginCache(const redisContext* redisConnect)
	:_redisConnect(redisConnect)
{}

UserLoginCache::~UserLoginCache()
{}


int UserLoginCache::getUserLoginCacheByUserName(const string& userName, UserLoginCacheBO& userLoginCache)
{
	//获取clientUid
	uint64_t clientUid;
	string getClientUidCmd = "lindex " + userName + " 0";
	int ret = getUint64ValueByCmd(getClientUidCmd, clientUid);
	if(ret != 0)
	{
		LOG_ERROR("getUint64ValueByCmd failed! cmd:%s", getClientUidCmd.c_str());
		return -1;
	}

	//获取状态
	int status;
	string getStatusCmd = "lindex " + userName + " 1";
	ret = getIntValueByCmd(getStatusCmd, status);
	if(ret != 0)
	{
		LOG_ERROR("getIntValueByCmd failed! cmd:%s", getStatusCmd.c_str());
		return -1;
	}

	LOG_INFO("[getUserLoginCacheByUserName]: userName:%s, clientUid:%lu, status:%d", userName.c_str(), clientUid, status);
	userLoginCache.setUserName(userName);
	userLoginCache.setClientUid(clientUid);
	userLoginCache.setStatus(status);
	return 0;
}

void UserLoginCache::cacheUserLoginInfo(UserLoginCacheBO userLoginCache, int ttl)
{
	//插入或者更新用户登录缓存
	string isExistUserNameCacheCmd = "llen " + userLoginCache.getUserName();
	redisReply* reply = (redisReply*)redisCommand(const_cast<redisContext*>(_redisConnect), isExistUserNameCacheCmd.c_str());
	if(reply->integer == 0)
	{
		//插入
		std::vector<std::string> cmds;
		cmds.push_back("rpush");
		cmds.push_back(userLoginCache.getUserName());
		cmds.push_back(type2str(userLoginCache.getClientUid()));
		cmds.push_back(type2str(userLoginCache.getStatus()));

		cmds.push_back("expire");
		cmds.push_back(userLoginCache.getUserName());
		cmds.push_back(type2str(ttl));

		(void)executeCmds(cmds);
	}
	else
	{
		//更新
		std::vector<std::string> cmds;
		cmds.push_back("lset");
		cmds.push_back(userLoginCache.getUserName());
		cmds.push_back("0");
		cmds.push_back(type2str(userLoginCache.getClientUid()));

		cmds.push_back("lset");
		cmds.push_back(userLoginCache.getUserName());
		cmds.push_back("1");
		cmds.push_back(type2str(userLoginCache.getStatus()));

		cmds.push_back("expire");
		cmds.push_back(userLoginCache.getUserName());
		cmds.push_back(type2str(ttl));

		(void)executeCmds(cmds);
	}
}

void UserLoginCache::updateCacheTtl(const string& key, int ttl)
{
	string cmd = "expire " + key + " " + type2str(ttl);
	justExecuteCmd(cmd);
}

void UserLoginCache::justExecuteCmd(const string& cmd)
{
	redisReply* reply = (redisReply*)redisCommand(const_cast<redisContext*>(_redisConnect), cmd.c_str());
	freeReplyObject(reply);
}

int UserLoginCache::executeCmds(const vector<string>& cmds)
{
	vector<const char*> argv;
    vector<size_t> argvlen;
    argv.reserve(cmds.size());
    argvlen.reserve(cmds.size());

    ostringstream cmd;
    for(vector<string>::const_iterator it = cmds.begin(); it != cmds.end();++it) 
    {
        argv.push_back(it->c_str());
        argvlen.push_back(it->size());

        cmd << *it;
        if (argv.size() <  cmds.size()) {
            cmd << " ";
        }
    }
    LOG_INFO("cmds:%s", cmd.str().c_str());//TODO 去掉调试信息

    redisReply* reply = (redisReply*)redisCommandArgv(const_cast<redisContext*>(_redisConnect), static_cast<int>(cmds.size()), argv.data(), argvlen.data());
    if(reply == NULL)
    {
    	LOG_ERROR("redisCommand failed! cmd:%s, errmsg:%s", cmd.str().c_str(), _redisConnect->errstr);
		return -1;
    }

    if(reply->type == REDIS_REPLY_ERROR)
    {
    	LOG_ERROR("redisCommand failed! cmd:%s, errmsg:%s", cmd.str().c_str(), reply->str);
		return -1;
    }

    return 0;
}


int UserLoginCache::getStringValueByCmd(const string& cmd, string& value)
{
	redisReply* reply = (redisReply*)redisCommand(const_cast<redisContext*>(_redisConnect), cmd.c_str());
	if(reply == NULL)
	{
		LOG_ERROR("redisCommand failed! cmd:%s, errmsg:%s", cmd.c_str(), _redisConnect->errstr);
		return -1;
	}

	if(reply->type != REDIS_REPLY_STRING)
	{
		LOG_ERROR("redisCommand return type is error! type:%d", reply->type);
		freeReplyObject(reply);
		return -1;
	}

	value = reply->str;
	freeReplyObject(reply);
	return 0;
}

int UserLoginCache::getUint64ValueByCmd(const string& cmd, uint64_t& value)
{
	redisReply* reply = (redisReply*)redisCommand(const_cast<redisContext*>(_redisConnect), cmd.c_str());
	if(reply == NULL)
	{
		LOG_ERROR("redisCommand failed! cmd:%s, errmsg:%s", cmd.c_str(), _redisConnect->errstr);
		return -1;
	}

	if(reply->type == REDIS_REPLY_INTEGER)
	{
		value = reply->integer;
		freeReplyObject(reply);
		return 0;
	}
	else if(reply->type == REDIS_REPLY_STRING)
	{
		value = strToUint64(reply->str);
		freeReplyObject(reply);
		return 0;
	}
	else
	{
		LOG_ERROR("redisCommand return type is error! type:%d", reply->type);
		freeReplyObject(reply);
		return -1;
	}
}

int UserLoginCache::getIntValueByCmd(const string& cmd, int& value)
{
	redisReply* reply = (redisReply*)redisCommand(const_cast<redisContext*>(_redisConnect), cmd.c_str());
	if(reply == NULL)
	{
		LOG_ERROR("redisCommand failed! cmd:%s, errmsg:%s", cmd.c_str(), _redisConnect->errstr);
		return -1;
	}

	if(reply->type == REDIS_REPLY_INTEGER)
	{
		value = reply->integer;
		freeReplyObject(reply);
		return 0;
	}
	else if(reply->type == REDIS_REPLY_STRING)
	{
		value = strToLongInt(reply->str);
		freeReplyObject(reply);
		return 0;
	}
	else
	{
		LOG_ERROR("redisCommand return type is error! type:%d", reply->type);
		freeReplyObject(reply);
		return -1;
	}
}

inline uint64_t UserLoginCache::strToUint64(const string& str)
{
	return strtoull(str.c_str(), NULL, 10);
}

inline uint64_t UserLoginCache::strToLongInt(const string& str)
{
	return strtol(str.c_str(), NULL, 10);
}

template <class T>
inline string UserLoginCache::type2str(T v)
{
    stringstream s;
    s << v;
    return s.str();
}
