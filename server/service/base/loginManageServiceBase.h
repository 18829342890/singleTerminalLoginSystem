#pragma once

#include <string>
#include <cppconn/connection.h>

using namespace std;

class LoginManageServiceBase
{
public:
	LoginManageServiceBase(const sql::Connection* mysqlConnect);
	~LoginManageServiceBase();

	int getCode();
	string getMsg();

protected:
	int _code;
	string _msg;
	const sql::Connection* _mysqlConnect;
};