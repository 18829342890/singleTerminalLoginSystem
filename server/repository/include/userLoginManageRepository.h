#pragma once
#include "mylib/mylibLog/logrecord.h"
#include "mylib/mylibSql/sqlApi.h"
#include "server/domain/userLoginManageBO.h"


namespace userLoginService{
namespace repository{

using namespace userLoginService::BO;


class UserLoginManageRepository
{
public:
	UserLoginManageRepository(SqlApi sqlApi)
		:_sqlApi(sqlApi)
	{}
	virtual ~UserLoginManageRepository(){}

	int insert(UserLoginManageBO userLoginManage);//unused
	int update(UserLoginManageBO userLoginManage);//unused
	//获取用户是否已登录
	int getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined);
	//获取已登录用户的客户端信息
	int getAlreadyLoginedClientInfo(const string& userName, string& ip, int& port);
	//设置用户的登录状态
	int setStatusByUserName(const string& userName, int status);
	//更新用户的客户端信息
	int updateClientInfoByUserName(const string& userName, const string& ip, int port);

private:
	SqlApi _sqlApi;
};



}
}