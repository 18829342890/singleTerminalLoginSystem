#include "logout.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/domain/userLoginManageBO.h"
#include "server/infrastructure/include/userLoginCache.h"
#include "logout.h"
#include "server/repository/include/userLoginManageRepository.h"
#include <sstream>

using namespace userLoginSystem::myEnum;
using namespace userLoginService::BO;
using namespace userLoginService::repository;
using namespace userLoginService::infrastructure;


Logout::Logout(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoCacheTtl) 
		:LoginManageServiceBase(sqlApi, redisConnect),
		 _userLoginInfoCacheTtl(userLoginInfoCacheTtl)
{}

Logout::~Logout()
{}

int Logout::processLogout(const string& userName, const string& clientUid)
{
	UserLoginManageBO userLoginManage;
	UserLoginManageRepository userLoginManageRepository(_sqlApi);
	int ret = userLoginManageRepository.select(userName, userLoginManage);
	if(ret != 0)
	{
		LOG_ERROR("userLoginManageRepository.select failed!");
		_code = DB_OP_FAILED;
		_msg = "select data from DB failed!";
		return -1;
	}

	//判断是否已退出登录 需判断clientuid，防止另一个退出登录的请求由于网络延迟慢到了，导致记录的clientUid不一致的情况
	if(clientUid == userLoginManage.getClientUid() && userLoginManage.getStatus() == LOGOUT)
	{
		_code = SUCCESS;
		_msg = "logout success!";
		LOG_INFO("already logout! just return success.");
		return 0;
	}

	//设置为退出登录状态
	ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, clientUid, LOGOUT);
	if(ret != 0)
	{
		LOG_ERROR("updateClientUidAndStatusByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "update data into DB failed!";
		return -1;
	}

	_code = SUCCESS;
	_msg = "logout success!";
	LOG_INFO("logout success!");
	return 0;
}

void Logout::cacheUserLogoutInfo(const string& userName, const string& clientUid)
{
	//缓存用户已退出登录，并设置登录时间
	UserLoginCacheBO userLoginCacheBO;
	userLoginCacheBO.setUserName(userName);
	userLoginCacheBO.setClientUid(clientUid);
	userLoginCacheBO.setStatus(LOGOUT);

	UserLoginCache userLoginCache(_redisConnect);
	userLoginCache.cacheUserLoginInfo(userLoginCacheBO, _userLoginInfoCacheTtl);
}














































