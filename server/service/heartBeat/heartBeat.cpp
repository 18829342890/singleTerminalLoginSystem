#include "heartBeat.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/infrastructure/include/userLoginCache.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::infrastructure;

HeartBeat::HeartBeat(const SqlApi& sqlApi, const redisContext* redisConnect, int userLoginInfoTtl)
	:LoginManageServiceBase(sqlApi, redisConnect),
	 _userLoginInfoTtl(userLoginInfoTtl)
{}

HeartBeat::~HeartBeat()
{}



int HeartBeat::processHeartBeat(const string& userName, const string& clientUid, int& clientOperation)
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
		//当前用户还在登录中，状态正常
		_code = SUCCESS;
		_msg = "success";
		clientOperation = NOTHING;
		return 0;
	}

	if(clientUid == userLoginCache.getClientUid() && userLoginCache.getStatus() == LOGOUT)
	{
		//当前用户已退出登录
		_code = SUCCESS;
		_msg = "success";
		clientOperation = CURRENT_USER_LOGOUT;
		return 0;
	}

	if(userLoginCache.getClientUid() == "0" && userLoginCache.getStatus() == LOGOUT)
	{
		//被管理员踢出登录，当前用户应退出登录
		_code = SUCCESS;
		_msg = "success";
		clientOperation = KICKOUT_BY_MANAGER_JUST_LOGOUT;
		return 0;
	}

	if(clientUid != userLoginCache.getClientUid() || userLoginCache.getStatus() == LOGOUT)
	{
		//当前账号在另一台设备登录，当前设备应退出登录
		_code = SUCCESS;
		_msg = "success";
		clientOperation = OTHER_DEVICE_LOGINED_JUST_LOGOUT;
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