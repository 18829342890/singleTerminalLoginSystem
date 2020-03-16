#pragma once

#include "mylib/myLibEncrypt/bcrypt.h"
#include "server/service/base/loginManageServiceBase.h"
#include <string>

using namespace std;


class Loginer : public LoginManageServiceBase
{
public:
	Loginer(const sql::Connection* mysqlConnect, const redisContext* redisConnect, int userLoginInfoCacheTtl);
	virtual ~Loginer();

	int processLogin(const string& userName, const string& passWord, const string& clientUid);
	void cacheUserLoginInfo(const string& userName, const string& clientUid);

private:
	//用户名和密码是否有效
	int isValidAndPassWordByUserName(const string& userName, const string& passWord, bool& isValid);
	//登录
	int login(const string& userName, const string&  clientUid);

private:
	int _userLoginInfoCacheTtl;

};




