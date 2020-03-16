#pragma once

#include <mysql_connection.h>
#include "mylib/mylibLog/logrecord.h"
#include "mylib/mylibSql/sqlApi.h"
#include "server/domain/userLoginManageBO.h"


namespace userLoginService{
namespace repository{

using namespace userLoginService::BO;


class UserLoginManageRepository
{
public:
	UserLoginManageRepository(const sql::Connection* mysqlConnect);
	virtual ~UserLoginManageRepository();

	int select(const string& userName, UserLoginManageBO& userLoginManage);
	int insert(const UserLoginManageBO& userLoginManage);//unused
	int update(const UserLoginManageBO& userLoginManage);//unused
	//获取用户是否已登录
	int getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined);
	//设置用户的登录状态
	int updateClientUidAndStatusByUserName(const string& userName, const string& clientUid, int status);

private:
	const sql::Connection* _mysqlConnect;
};



}
}