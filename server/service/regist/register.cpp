#include "register.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "mylib/myLibEncrypt/base64.h"
#include "server/repository/include/userPasswordRepository.h"
#include <sstream>
#include <regex>
#include <grpcpp/grpcpp.h>

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;



Register::Register(const sql::Connection* mysqlConnect, int saltWorkFactor) 
		:LoginManageServiceBase(mysqlConnect),
		 _saltWorkFactor(saltWorkFactor)
{}

Register::~Register()
{}


int Register::processRegist(const string& userName, const string& passWord)
{
	//检查密码格式
	if(!isLegalPassWordFormat(passWord))
	{
		LOG_ERROR("CheckPassWordFormatIsLegal failed!");
		return -1;
	}

	//校验是否已经注册
	bool isAlreadyRegistFlag = false;
	UserPasswordRepository userPasswordRepository(_mysqlConnect);
	int ret = userPasswordRepository.isAlreadyRegist(userName, isAlreadyRegistFlag);
	if(ret != 0)
	{
		LOG_INFO("isAlreadyRegist failed! userName:%s", userName.c_str());
		_code = DB_OP_FAILED;
		_msg = "system error: select data from DB failed!";
		return -1;
	}

	if(isAlreadyRegistFlag)
	{
		LOG_INFO("[%s] already regist!", userName.c_str());
		_code = SUCCESS;
		_msg =  userName + " already regist!";
		return 0;
	}

	//入库
	if(regist(userName, passWord) != 0)
	{
		LOG_ERROR("process regist failed!");
		return -1;
	}

	_code = SUCCESS;
	_msg = "regist success!";
	return 0;
}

bool Register::isLegalPassWordFormat(const string& passWord)
{
	//检查长度
	if(passWord.length() < 6 or passWord.length() > 20)
	{
		LOG_ERROR("length is < 6 or > 20! length:%d", passWord.length());
		_code = PASSWORD_FORMAT_ERROR;
		_msg = "pass word length is between 6 and 20.";
		return false;
	}

	//只能是字母、数字、下划线的组合
	regex reg("^[A-Za-z0-9_]+$");
	if(!regex_match(passWord.c_str(), reg))
	{
		LOG_ERROR("passWord do not match ^[A-Za-z0-9_]+$");
		_code = PASSWORD_FORMAT_ERROR;
		_msg = "passWord do not match ^[A-Za-z0-9_]+$";
		return false;
	}

	return true;
}


int Register::regist(const string& userName, const string& passWord)
{
	//生成盐
	char salt[BCRYPT_HASHSIZE];
	char passWordHash[BCRYPT_HASHSIZE];
	int ret = bcrypt_gensalt(_saltWorkFactor, salt);
	if(ret != 0)
	{
		LOG_ERROR("gen salt failed!");
		_code = GEN_SALT_FAILED;
		_msg = "gen salt for password failed!";
		return -1;
	}

	//加密密码
	clock_t before = clock();
	ret = bcrypt_hashpw(passWord.c_str(), salt, passWordHash);
	if(ret != 0)
	{
		LOG_ERROR("bcrypt_hashpw failed!");
		_code = ENCRYPT_FAILED;
		_msg = "bcrypt failed!";
		return -1;
	}

	clock_t after = clock();
	LOG_INFO("bcrypt_hashpw time taken: %f seconds; passWordHash:%s", (float)(after - before) / CLOCKS_PER_SEC, passWordHash);

	//insert
	UserPassWordBO userPassWord;
	userPassWord.setUserName(userName);
	userPassWord.setPassWord(passWordHash);
	userPassWord.setSalt(salt);

	UserPasswordRepository userPasswordRepository(_mysqlConnect);
	ret = userPasswordRepository.insert(userPassWord);
	if(ret != 0)
	{
		LOG_ERROR("insert into UserPasswordRepository failed!");
		_code = DB_OP_FAILED;
		_msg = "SYSTEM ERROR: insert data into DB failed!";
		return -1;
	}

	return 0;
}














