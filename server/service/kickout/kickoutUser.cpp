#include "kickoutUser.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;



KickoutUser::KickoutUser(const sql::Connection* mysqlConnect) 
		:LoginManageServiceBase(mysqlConnect)
{}

KickoutUser::~KickoutUser()
{}

int KickoutUser::processkickout(const string& userName)
{
	//TODO 加权限管理
	//获取用户登录信息
	// UserLoginCacheBO userLoginCache;
	// int ret = getUserLoginInfo(userName, userLoginCache);
	// if(ret == -1)
	// {
	// 	LOG_ERROR("get user login info from cache and DB failed!");
	// 	_code = SYSTEM_ERROR;
	// 	_msg = "get user login info from cache and DB failed!";
	// 	return -1;
	// }
	// else if(ret == -2)
	// {
	// 	LOG_INFO("this user never login! keckout success.");
	// 	_code = SUCCESS;
	// 	_msg = "this user never login! keckout success.";
	// 	return 0;
	// }

	// //已被踢出
	// if(userLoginCache.getClientUid() == "0" && userLoginCache.getStatus() == LOGOUT)
	// {
	// 	LOG_INFO("this user already kickout.");
	// 	_code = SUCCESS;
	// 	_msg = "success";
	// 	return 0;
	// }

	//设置为退出登录状态 clientUid为0，记录是被踢出的
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	int ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, "0", LOGOUT);
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