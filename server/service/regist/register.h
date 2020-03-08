#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <string>
#include "mylog/mylibSql/sqlApi.h"
using namespace std;


class Register
{

public:
	Register(SqlApi& sqlApi, int saltWorkFactor);
	virtual ~Register();

	int processRegist(const string& userName, const string& passWord, int& code, string& msg);

private:
	bool isLegalPassWordFormat(const string& passWord, string& errmsg);
	int regist(const string& userName, const string& passWord);

private:
	SqlApi _sqlApi;
	int _saltWorkFactor;
};







#endif