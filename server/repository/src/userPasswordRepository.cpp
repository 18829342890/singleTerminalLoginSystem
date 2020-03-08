#include "userPasswordRepository.h"
#include <sstream>

using namespace userLoginService::repository;

int UserPasswordRepository::insert(UserPassWordBO userPassWord)
{
	std::stringstream ss;
	ss << "insert into user.t_user_password (FstrUserName, FstrPassWord, FstrSalt, FuiCreateTime) values ('" 
	   << userPassWord.getUserName() << "', '" << userPassWord.getPassWord() << "', '" << userPassWord.getSalt() << "', " << time(NULL) << ")";

	if(_sqlApi.insert(ss.str()) != 0)
	{
		LOG_ERROR("insert failed! insert string:%s", ss.str().c_str());
		return -1;
	}

	return 0;
}


int UserPasswordRepository::isAlreadyRegist(const string& userName, bool& isAlreadyRegistFlag)
{
	std::stringstream ss;
	ss << "select count(1) from user.t_user_password where FstrUserName = '" << userName << "'";

	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 1);
	int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	LOG_INFO("userName:%s, count(1):%d", userName.c_str(), count);

	isAlreadyRegistFlag = count == 0 ? false : true;
	return 0;
}

int UserPasswordRepository::getSaltByUserName(const string& userName, char salt[BCRYPT_HASHSIZE])
{
	//select
	std::stringstream ss;
	ss << "select FstrSalt from user.t_user_password where FstrUserName = '" << userName << "'";

	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	//获取结果
	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	//判断是否存在该用户
	if(mysqlResult.rowsCount <= 0)
	{
		LOG_ERROR("is not exist this userName! userName:%s", userName.c_str());
		return -1;
	}

	//获取salt
	assert(mysqlResult.fieldsCount == 1);
	string saltString = mysqlResult.mysqlRowVec[0][0];
	if(saltString.length() > BCRYPT_HASHSIZE - 1)
	{
		LOG_ERROR("the salt length in DB is error! length:%d, salt:%s", saltString.length(), saltString.c_str());
		return -1;
	}

	strcpy(salt, saltString.c_str());
	LOG_INFO("userName:%s, salt:%s", userName.c_str(), salt);
	return 0;
}

int UserPasswordRepository::isValidUserNameAndPassWord(const string& userName, const string& passWordHash, bool& isValid)
{
	//select
	std::stringstream ss;
	ss << "select count(1) from user.t_user_password where FstrUserName = '" << userName << "' and FstrPassWord = '" << passWordHash << "'";

	int ret = _sqlApi.select(ss.str());
	if(ret != 0)
	{
		LOG_ERROR("select failed! select string:%s", ss.str().c_str());
		return -1;
	}

	//获取结果
	mysql_result_t mysqlResult;
	_sqlApi.getResult(mysqlResult);

	//判断是否有效
	assert(mysqlResult.rowsCount == 1);
	assert(mysqlResult.fieldsCount == 1);
	int count = strtoul(mysqlResult.mysqlRowVec[0][0], NULL, 0);
	isValid = count == 0 ? false : true;

	LOG_INFO("userName:%s, passWordHash:%s, isValid:%d", userName.c_str(), passWordHash.c_str(), isValid);
	return 0;
}