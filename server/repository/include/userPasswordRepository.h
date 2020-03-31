#pragma once

#include <string>
#include <mysql_connection.h>
#include "mylib/mylibLog/logrecord.h"
#include "mylib/mylibSql/sqlApi.h"
#include "mylib/myLibEncrypt/bcrypt.h"
#include "server/domain/userPassWordBO.h"

namespace userLoginService{
namespace repository{

using namespace userLoginService::BO;

class UserPasswordRepository
{

public:
	UserPasswordRepository(const sql::Connection* mysqlConnect);
	virtual ~UserPasswordRepository();

	
	int insert(UserPassWordBO userPassWord);
	int update(UserPassWordBO userPassWord); //unused
	//获取用户是否已注册
	int isAlreadyRegist(const string& userName, bool& isAlreadyRegistFlag);
	//获取用户加密所用的盐
	int selectSaltByUserName(const string& userName, char salt[BCRYPT_HASHSIZE]);
	//用户名密码是否正确
	int isValidPassWordByUserName(const string& userName, const string& passWordHash, bool& isValid);

private:
	const sql::Connection* _mysqlConnect;
};



}
}