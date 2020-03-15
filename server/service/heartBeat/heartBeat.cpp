#include "heartBeat.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/infrastructure/include/userLoginCache.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::infrastructure;
using namespace userLoginService::repository;

HeartBeat::HeartBeat(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoTtl)
	:LoginManageServiceBase(sqlApi, redisConnect),
	 _userLoginInfoTtl(userLoginInfoTtl)
{}

HeartBeat::~HeartBeat()
{}



int HeartBeat::processHeartBeat(const string& userName, uint64_t clientUid, int& clientOperation)
{
	UserLoginCacheBO userLoginCache;
	int ret = getUserLoginInfo(userName, userLoginCache);
	if(ret != 0)
	{
		_code = SYSTEM_ERROR;
		_msg = "get user login info from cache or DB failed!";
		clientOperation = NOTHING;
		return -1;
	}

	if(clientUid == userLoginCache.getClientUid() && userLoginCache.getStatus() == LOGINED)
	{
		_code = SUCCESS;
		_msg = "success";
		clientOperation = NOTHING;
		return 0;
	}

	if(clientUid != userLoginCache.getClientUid() || userLoginCache.getStatus() == LOGOUT)
	{
		_code = SUCCESS;
		_msg = "success";
		clientOperation = LOG_OUT;
		return 0;
	}

	_code = SUCCESS;
	_msg = "success";
	clientOperation = NOTHING;
	return 0;
}

void HeartBeat::updateUserLoginInfoCacheTtl(const string& userName)
{
	UserLoginCache userLoginCache(_redisConnect);
	userLoginCache.updateCacheTtl(userName, _userLoginInfoTtl);
}


int HeartBeat::getUserLoginInfo(const string& userName, UserLoginCacheBO& userLoginCacheBO)
{
	//先从缓存中获取登录信息
	UserLoginCache userLoginCache(_redisConnect);
	int ret = userLoginCache.getUserLoginCacheByUserName(userName, userLoginCacheBO);
	if(ret == 0)
	{
		return 0;
	}

	LOG_WARNNING("get User Login info from Cache failed!");

	//缓存获取失败，再从DB中获取登录信息
	UserLoginManageBO userLoginManage;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	ret = userLoginManageRepository.select(userName, userLoginManage);
	if(ret != 0)
	{
		LOG_ERROR("not found this userName info from cache and DB! userName:%s", userName.c_str());
		return -1;
	}

	userLoginCacheBO.setUserName(userLoginManage.getUserName());
	userLoginCacheBO.setClientUid(userLoginManage.getClientUid());
	userLoginCacheBO.setStatus(userLoginManage.getStatus());
	return 0;
}