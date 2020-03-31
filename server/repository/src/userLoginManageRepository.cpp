#include "server/repository/include/userLoginManageRepository.h"
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <boost/scoped_ptr.hpp>
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>

using namespace userLoginService::repository;


UserLoginManageRepository::UserLoginManageRepository(const sql::Connection* mysqlConnect)
		:_mysqlConnect(mysqlConnect)
{}

UserLoginManageRepository::~UserLoginManageRepository()
{}


int UserLoginManageRepository::select(const string& userName, UserLoginManageBO& userLoginManage)
{
	try{
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"select FuiId, FstrUserName, FstrClientUid, FuiStatus, FuiCreateTime from user.t_user_login_manage where FstrUserName = ?"));
		prepStmt->setString(1, userName);

		boost::scoped_ptr< sql::ResultSet > res(prepStmt->executeQuery());
		if(res->rowsCount() <= 0)
		{
			LOG_ERROR("is not exist this userName! userName:%s", userName.c_str());
			return -1;
		}

		res->next();
		userLoginManage.setId(res->getUInt64("FuiId"));
		userLoginManage.setUserName(res->getString("FstrUserName"));
		userLoginManage.setClientUid(res->getString("FstrClientUid"));
		userLoginManage.setStatus(res->getInt("FuiStatus"));
		userLoginManage.setCreateTime(res->getUInt("FuiCreateTime"));
		return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}
}


int UserLoginManageRepository::getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined)
{
	try{
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"select count(1) from user.t_user_login_manage where FstrUserName = ? and FuiStatus = 1"));
		prepStmt->setString(1, userName);

		boost::scoped_ptr< sql::ResultSet > res(prepStmt->executeQuery());
		res->next();
		int count = res->getInt(1);
		LOG_INFO("userName:%s, status =1`s count:%d", userName.c_str(), count);

		isAlreadyLogined = count == 0 ? false : true;
		return 0;
	}catch(sql::SQLException &e) {
		LOG_ERROR("occur SQLException! errmsg:%s, mysql err code:%d, SQLState:%s", e.what(), e.getErrorCode(), e.getSQLState().c_str());
		return -1;
	}catch(std::runtime_error &e){
		LOG_ERROR("occur runtime_error! errmsg:%s", e.what());
		return -1;
	}
}

int UserLoginManageRepository::updateClientUidAndStatusByUserName(const string& userName, const string& clientUid, int status)
{
	try {
		boost::scoped_ptr< sql::PreparedStatement > prepStmt(const_cast<sql::Connection*>(_mysqlConnect)->prepareStatement(
		 	"insert into user.t_user_login_manage (FstrUserName,FstrClientUid,FuiStatus,FuiCreateTime) values (?, ?, ?, ?)  ON DUPLICATE KEY UPDATE FstrUserName = ?, FstrClientUid= ?, FuiStatus = ?"));
		 prepStmt->setString(1, userName);
		 prepStmt->setString(2, clientUid);
		 prepStmt->setInt(3, status);
		 prepStmt->setUInt(4, time(NULL));
		 prepStmt->setString(5, userName);
		 prepStmt->setString(6, clientUid);
		 prepStmt->setInt(7, status);

		 int insertCount = prepStmt->executeUpdate();
		 if(insertCount <= 0)
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