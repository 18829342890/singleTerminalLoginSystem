#ifndef __SQL_API_H__
#define __SQL_API_H__

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<vector>
#include<mysql/mysql.h>
using namespace std;


typedef struct MYSQL_RESULT
{
	vector<MYSQL_ROW> mysqlRowVec;
	unsigned long rowsCount;
	unsigned int  fieldsCount; 

}mysql_result_t;

class SqlApi
{
public:
	SqlApi();
	virtual ~SqlApi();
	int connectMysql();                                        
	int closeMysql();
	void getResult(mysql_result_t& result);
	int select(string selectString);
	int insert(string insertString);
	int update(string updateString);                                          

private:
	void cleanResult();

private:
	MYSQL* _mysql;
	string _host;
	string _user;
	string _passwd;
	string _db;
	short _port;
	mysql_result_t _mysqlResult;
};



#endif


