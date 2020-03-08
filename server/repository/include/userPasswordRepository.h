#pragma once
#include "logrecord.h"
#include "sqlApi.h"
#include "bcrypt.h"
#include "server/domain/userPassWordBO.h"

namespace userLoginService{
namespace repository{

using namespace userLoginService::BO;

class UserPasswordRepository
{

public:
	UserPasswordRepository(SqlApi sqlApi)
		:_sqlApi(sqlApi)
	{}
	virtual ~UserPasswordRepository(){}

	int insert(UserPassWordBO userPassWord);
	int update(UserPassWordBO userPassWord); //unused
	//获取用户是否已注册
	int isAlreadyRegist(const string& userName, bool& isAlreadyRegistFlag);
	//获取用户加密所用的盐
	int getSaltByUserName(const string& userName, char salt[BCRYPT_HASHSIZE]);
	//用户名密码是否正确
	int isValidUserNameAndPassWord(const string& userName, const string& passWordHash, bool& isValid);

private:
	SqlApi _sqlApi;
};



}
}