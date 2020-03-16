#include "loginManageServiceBase.h"
#include "mylib/enum/code.h"
#include "server/infrastructure/include/userLoginCache.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::infrastructure;
using namespace userLoginService::repository;

LoginManageServiceBase::LoginManageServiceBase(const SqlApi& sqlApi, const redisContext* redisConnect)
	:_code(SUCCESS),
	 _msg("success"),
	 _sqlApi(sqlApi),
	 _redisConnect(redisConnect)
{}


LoginManageServiceBase::~LoginManageServiceBase()
{}


int LoginManageServiceBase::getCode()
{
	return _code;
}

string LoginManageServiceBase::getMsg()
{
	return _msg;
}


int LoginManageServiceBase::getUserLoginInfo(const string& userName, UserLoginCacheBO& userLoginCacheBO)
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
		return ret;
	}

	userLoginCacheBO.setUserName(userLoginManage.getUserName());
	userLoginCacheBO.setClientUid(userLoginManage.getClientUid());
	userLoginCacheBO.setStatus(userLoginManage.getStatus());
	return 0;
}