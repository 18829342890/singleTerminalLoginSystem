#include "server/repository/include/userPasswordRepository.h"
#include <sstream>
#include <assert.h>
#include <boost/scoped_ptr.hpp>
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>

using namespace userLoginService::repository;


UserPasswordRepository::UserPasswordRepository(const sql::Connection* mysqlConnect)
		:_mysqlConnect(mysqlConnect)
{}

UserPasswordRepository::~UserPasswordRepository()
{}


int UserPasswordRepository::insert(UserPassWordBO userPassWord)
{
	try {
		 boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"insert into user.t_user_password (FstrUserName, FstrPassWord, FstrSalt, FuiCreateTime) values (?, ?, ?, ?)"));
		 prepStmt->setString(1, userPassWord.getUserName());
		 prepStmt->setString(2, userPassWord.getPassWord());
		 prepStmt->setString(3, userPassWord.getSalt());
		 prepStmt->setUInt(4, time(NULL));

		 int insertCount = prepStmt->executeUpdate();
		 LOG_INFO("insertCount:%d", insertCount);
		 if(insertCount != 1)
		 {
		 	LOG_ERROR("insert failed! insertCount:%d", insertCount);
			return -1;
		 }

		 return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}
}


int UserPasswordRepository::isAlreadyRegist(const string& userName, bool& isAlreadyRegistFlag)
{
	try {
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"select count(1) from user.t_user_password where FstrUserName = ?"));
		prepStmt->setString(1, userName);

		boost::scoped_ptr< sql::ResultSet > res(prepStmt->executeQuery());
		res->next();
		int count = res->getInt(1);
		LOG_INFO("userName:%s, count(1):%d", userName.c_str(), count);

		isAlreadyRegistFlag = count == 0 ? false : true;
		return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}
}

int UserPasswordRepository::selectSaltByUserName(const string& userName, char salt[BCRYPT_HASHSIZE])
{
	try{
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"select FstrSalt from user.t_user_password where FstrUserName = ?"));
		prepStmt->setString(1, userName);

		boost::scoped_ptr< sql::ResultSet > res(prepStmt->executeQuery());
		if(res->rowsCount() <= 0)
		{
			LOG_ERROR("is not exist this userName! userName:%s", userName.c_str());
			return -1;
		}

		res->next();
		string saltString = res->getString("FstrSalt");
		if(saltString.length() > BCRYPT_HASHSIZE - 1)
		{
			LOG_ERROR("the salt length in DB is error! length:%d, salt:%s", saltString.length(), saltString.c_str());
			return -1;
		}

		strcpy(salt, saltString.c_str());
		LOG_INFO("userName:%s, salt:%s", userName.c_str(), salt);
		return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}
}

int UserPasswordRepository::isValidAndPassWordByUserName(const string& userName, const string& passWordHash, bool& isValid)
{
	try{
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"select count(1) from user.t_user_password where FstrUserName = ? and FstrPassWord = ?"));
		prepStmt->setString(1, userName);
		prepStmt->setString(2, passWordHash);

		boost::scoped_ptr< sql::ResultSet > res(prepStmt->executeQuery());
		res->next();
		int count = res->getInt(1);
		isValid = count == 0 ? false : true;

		LOG_INFO("userName:%s, passWordHash:%s, isValid:%d", userName.c_str(), passWordHash.c_str(), isValid);
		return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}

	// select
	// std::stringstream ss;
	// ss << "select count(1) from user.t_user_password where FstrUserName = '" << userName << "' and FstrPassWord = '" << passWordHash << "'";

	// int ret = _sqlApi.select(ss.str());
	// if(ret != 0)
	// {
	// 	LOG_ERROR("select failed! select string:%s", ss.str().c_str());
	// 	return -1;
	// }

	// //获取结果
	// mysql_result_t mysqlResult;
	// _sqlApi.getResult(mysqlResult);

	// //判断是否有效
	// assert(mysqlResult.rowsCount == 1);
	// assert(mysqlResult.fieldsCount == 1);
	// int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	// isValid = count == 0 ? false : true;

	// LOG_INFO("userName:%s, passWordHash:%s, isValid:%d", userName.c_str(), passWordHash.c_str(), isValid);
	// return 0;
}