#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"
using namespace std;


class Register : public LoginManageServiceBase
{

public:
	Register(const sql::Connection* mysqlConnect, int saltWorkFactor);
	virtual ~Register();

	int processRegist(const string& userName, const string& passWord);

private:
	bool isLegalPassWordFormat(const string& passWord);
	int regist(const string& userName, const string& passWord);

private:
	int _saltWorkFactor;
};






