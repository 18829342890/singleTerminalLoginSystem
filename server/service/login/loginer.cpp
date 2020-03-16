#include <sstream>
#include <assert.h>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "loginer.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "server/domain/userLoginCacheBO.h"
#include "server/infrastructure/include/userLoginCache.h"
#include "server/repository/include/userPasswordRepository.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::BO;
using namespace userLoginService::repository;
using namespace userLoginService::infrastructure;





Loginer::Loginer(const sql::Connection* mysqlConnect, const redisContext* redisConnect, int userLoginInfoCacheTtl) 
		: LoginManageServiceBase(mysqlConnect, redisConnect),
		  _userLoginInfoCacheTtl(userLoginInfoCacheTtl)
{}

Loginer::~Loginer()
{}


int Loginer::processLogin(const string& userName, const string& passWord, const string& clientUid)
{
	//判断密码是否有效
	bool isValid = false;
	int ret = isValidAndPassWordByUserName(userName, passWord, isValid);
	if(ret < 0)
	{
		LOG_ERROR("isValidAndPassWordByUserName failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		return -1;
	}

	if(!isValid)
	{
		LOG_ERROR("userName or passWord error! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		_code = USERNAME_OR_PASSWD_ERROR;
		_msg = "userName or passWord error!";
		return -1;
	}

	//登录
	if(login(userName, clientUid) != 0)
	{
		LOG_ERROR("login failed! userName:%s, passWord:%s", userName.c_str(), passWord.c_str());
		_code = DB_OP_FAILED;
		_msg = "system error: insert or update data into DB failed!";
		return -1;
	}

	_code = SUCCESS;
	_msg = "login success!";
	LOG_INFO("userName:%s, clientUid:%s login success!", userName.c_str(), clientUid.c_str());
	return 0;
}

void Loginer::cacheUserLoginInfo(const string& userName, const string& clientUid)
{
	//缓存用户登录信息，并设置登录时间
	UserLoginCacheBO userLoginCacheBO;
	userLoginCacheBO.setUserName(userName);
	userLoginCacheBO.setClientUid(clientUid);
	userLoginCacheBO.setStatus(LOGINED);

	UserLoginCache userLoginCache(_redisConnect);
	userLoginCache.cacheUserLoginInfo(userLoginCacheBO, _userLoginInfoCacheTtl);
}

int Loginer::isValidAndPassWordByUserName(const string& userName, const string& passWord, bool& isValid)
{
	//获取该用户加密用的salt
	char salt[BCRYPT_HASHSIZE];
	UserPasswordRepository userPasswordRepository(_mysqlConnect);
	int ret = userPasswordRepository.selectSaltByUserName(userName, salt);
	if(ret != 0)
	{
		LOG_ERROR("getSaltByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	//加密密码
	char passWordHash[BCRYPT_HASHSIZE];
	ret = bcrypt_hashpw(passWord.c_str(), salt, passWordHash);
	if(ret != 0)
	{
		LOG_ERROR("bcrypt_hashpw failed!");
		_code = SYSTEM_ERROR;
		_msg = "system error: bcrypt failed!";
		return -1;
	}

	ret = userPasswordRepository.isValidAndPassWordByUserName(userName, passWordHash, isValid);
	if(ret != 0)
	{
		LOG_ERROR("isValidAndPassWordByUserName failed!");
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	return 0;
}

int Loginer::login(const string& userName, const string& clientUid)
{
	UserLoginManageRepository userLoginManageRepository(_mysqlConnect);
	return userLoginManageRepository.updateClientUidAndStatusByUserName(userName, clientUid, LOGINED);
}






