#include "logout.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/domain/userLoginManageBO.h"
#include "server/repository/include/userLoginManageRepository.h"


using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;


Logout::Logout(const sql::Connection* mysqlConnect) 
		:LoginManageServiceBase(mysqlConnect)
{}

Logout::~Logout()
{}

int Logout::processLogout(const string& userName, const string& clientUid)
{
	//设置为退出登录状态
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	int ret = userLoginManageRepository.updateClientUidAndStatusByUserName(userName, clientUid, LOGOUT);
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














































