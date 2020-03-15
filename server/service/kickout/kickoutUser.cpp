#include "kickoutUser.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/infrastructure/include/userLoginCache.h"
#include "server/repository/include/userLoginManageRepository.h"
#include <sstream>

using namespace userLoginSystem::myEnum;
using namespace userLoginService::BO;
using namespace userLoginService::repository;
using namespace userLoginService::infrastructure;



KickoutUser::KickoutUser(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoCacheTtl) 
		:LoginManageServiceBase(sqlApi, redisConnect),
		 _userLoginInfoCacheTtl(userLoginInfoCacheTtl)
{}

KickoutUser::~KickoutUser()
{}

int KickoutUser::processkickout(const string& userName)
{
	//TODO 加权限管理
	bool isAlreadyLogined = false;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	int ret = userLoginManageRepository.getIsAlreadyLogined(userName, isAlreadyLogined);
	if(ret != 0)
	{
		LOG_ERROR("getIsAlreadyLogined failed!");
		_code = DB_OP_FAILED;
		_msg = "select data from DB failed!";
		return -1;
	}

	//判断是否已退出登录 需判断clientuid，防止另一个退出登录的请求由于网络延迟慢到了，导致记录的clientUid不一致的情况
	if(!isAlreadyLogined)
	{
		_code = SUCCESS;
		_msg = "kickout success!";
		LOG_INFO("this user already logout! just return success.");
		return 0;
	}

	//设置为退出登录状态 clientUid为0，记录是被踢出的
	ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, 0, LOGOUT);
	if(ret != 0)
	{
		LOG_ERROR("updateClientUidAndStatusByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "update data into DB failed!";
		return -1;
	}

	_code = SUCCESS;
	_msg = "kickout success!";
	LOG_INFO("kickout success!");
	return 0;
}

void KickoutUser::cacheUserLogoutInfo(const string& userName)
{
	//缓存用户已退出登录，并设置登录时间
	UserLoginCacheBO userLoginCacheBO;
	userLoginCacheBO.setUserName(userName);
	userLoginCacheBO.setClientUid(0); //为被踢出的
	userLoginCacheBO.setStatus(LOGOUT);

	UserLoginCache userLoginCache(_redisConnect);
	userLoginCache.cacheUserLoginInfo(userLoginCacheBO, _userLoginInfoCacheTtl);
}