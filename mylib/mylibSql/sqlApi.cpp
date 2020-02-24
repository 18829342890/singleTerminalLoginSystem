#include "sqlApi.h"
#include "logrecord.h"
#include <errno.h>

SqlApi::SqlApi()
	:_mysql(NULL),
	_host("localhost"),
	_user("root"),
	_passwd("root"),
	_db("user"),
	_port(3306)
{}

SqlApi::SqlApi(string host, string user, string passwd, string db, short port)
	:_mysql(NULL),
	_host(host),    
	_user(user),
	_passwd(passwd),
	_db(db),
	_port(port)
{}


SqlApi::~SqlApi()
{
}

int SqlApi::connectMysql()
{
	if ((_mysql = mysql_init((MYSQL*)NULL)) == NULL)
	{
		LOG_ERROR("mysql_init failed! errno:%d, errmsg:%s",  mysql_errno(_mysql), mysql_error(_mysql));
		return -1;
	}

	if( !mysql_real_connect(_mysql, _host.c_str(), _user.c_str(), _passwd.c_str(), _db.c_str(), _port, 0, 0))
	{
		LOG_ERROR("mysql_real_connect failed! errno:%d, errmsg:%s",  mysql_errno(_mysql), mysql_error(_mysql));
		return -1;
	}
	
	LOG_INFO("connect to db success!");
	return 0;
}

int SqlApi::closeMysql()
{
	mysql_close(_mysql);
	return 0;
}

void SqlApi::getResult(mysql_result_t& result)
{
	result = _mysqlResult;
	cleanResult();
}

void SqlApi::cleanResult()
{
	vector<MYSQL_ROW>().swap(_mysqlResult.mysqlRowVec);
	//_mysqlResult.mysqlRowVec.swap();
	_mysqlResult.rowsCount = 0;
	_mysqlResult.fieldsCount = 0;
}

int SqlApi::select(string selectString)
{
	//查询
	if(mysql_query(_mysql, selectString.c_str()) != 0)
	{
		LOG_ERROR("mysql_query failed! errno:%d, errmsg:%s, selectString:%s",  mysql_errno(_mysql), mysql_error(_mysql), selectString.c_str());
		return -1;
	}

	//获取结果
	MYSQL_RES* res = mysql_store_result(_mysql);
	if(res == NULL)
	{
		LOG_ERROR("mysql_store_result failed! errno:%d, errmsg:%s, selectString:%s",  mysql_errno(_mysql), mysql_error(_mysql), selectString.c_str());
		return -1;
	}

	//设置_mysqlResult
	_mysqlResult.rowsCount = mysql_num_rows(res);
	_mysqlResult.fieldsCount = mysql_num_fields(res);

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		_mysqlResult.mysqlRowVec.push_back(row);
	}

	mysql_free_result(res);
	return 0;
}

int SqlApi::insert(string insertString)
{
	if(mysql_query(_mysql, insertString.c_str()) != 0)
	{
		LOG_ERROR("mysql_query failed! errno:%d, errmsg:%s, insertString:%s",  mysql_errno(_mysql), mysql_error(_mysql), insertString.c_str());
		return -1;
	}

	return 0;
}

int SqlApi::update(string updateString)
{
	if(mysql_query(_mysql, updateString.c_str()) != 0)
	{
		LOG_ERROR("mysql_query failed! errno:%d, errmsg:%s, updateString:%s",  mysql_errno(_mysql), mysql_error(_mysql), updateString.c_str());
		return -1;
	}

	return 0;
}




