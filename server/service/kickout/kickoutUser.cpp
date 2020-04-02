#include "kickoutUser.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;



KickoutUser::KickoutUser(const sql::Connection* mysqlConnect, RabbitmqClient& rabbitmqClient, const string& exchange, const string& routeKey) 
		:LoginManageServiceBase(mysqlConnect),
		 _rabbitmqClient(rabbitmqClient),
	     _exchange(exchange),
 	     _routeKey(routeKey)
{}

KickoutUser::~KickoutUser()
{}

int KickoutUser::processkickout(const string& userName)
{
	//TODO 加权限管理
	//获取用户登录信息
	UserLoginManageBO userLoginManage;
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	int ret = userLoginManageRepository.select(userName, userLoginManage);
	if(ret != 0)
	{
		LOG_ERROR("userLoginManageRepository.select failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	//设置为退出登录状态clientUid为0，记录是被踢出的
	ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, "0", LOGOUT);
	if(ret != 0)
	{
		LOG_ERROR("updateClientUidAndStatusByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "update data into DB failed!";
		return -1;
	}

	//通知用户退出登录
	ret = noticeUserLogout(userName, userLoginManage.getClientUid());
	if(ret != 0)
	{
		LOG_ERROR("noticeUserLogout failed!");
		return -1;
	}

	_code = SUCCESS;
	_msg = "kickout success!";
	LOG_INFO("kickout success!");
	return 0;
}

int KickoutUser::noticeUserLogout(const string& userName, const string& clientUid)
{
	//获取通知的消息
	string noticeMessage = getNoticeLogoutMsg(userName, clientUid, KICKOUT_BY_MANAGER_JUST_LOGOUT);

	//通知用户退出登录
	int ret = _rabbitmqClient.publish(noticeMessage, _exchange, _routeKey);
	if(ret != 0)
	{
		LOG_ERROR("publish message failed! userName:%s, clientUid:%s, noticeMessage:%s", userName.c_str(), clientUid.c_str(), noticeMessage.c_str());
		_code = PUBLISH_MSG_FAILED;
		_msg = "system error: publish message failed!";
		return -1;
	}

	LOG_INFO("publish message success! noticeMessage:%s", noticeMessage.c_str());
	return 0;
}











