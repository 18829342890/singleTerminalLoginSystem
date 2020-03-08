#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <string>
#include "sqlApi.h"
using namespace std;


class Register
{

public:
	Register(SqlApi& sqlApi);
	virtual ~Register();

	int processRegist(const string& userName, const string& passWord, int& code, string& msg);

private:
	bool CheckPassWordFormatIsLegal(const string& passWord, string& errmsg);
	int regist(const string& userName, const string& passWord);

private:
	SqlApi _sqlApi;
};







#endif