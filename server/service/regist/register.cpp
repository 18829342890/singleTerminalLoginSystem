#include "register.h"
#include "mylib/enum/code.h"
#include "mylib/mylibLog/logrecord.h"
#include "mylib/myLibEncrypt/base64.h"
#include "server/repository/userPasswordRepository.h"
#include <sstream>
#include <regex>
#include <grpcpp/grpcpp.h>

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

Register::Register(SqlApi& sqlApi, int saltWorkFactor) 
		:_sqlApi(sqlApi),
		 _saltWorkFactor(saltWorkFactor)
{}

Register::~Register()
{}

int Register::processRegist(const string& userName, const string& passWord, int& code, string& msg)
{
	//检查密码格式，防止sql注入
	if(!isLegalPassWordFormat(passWord, msg))
	{
		LOG_ERROR("CheckPassWordFormatIsLegal failed!");
		code = PASSWORD_FORMAT_ERROR;
		return -1;
	}

	//校验是否已经注册
	bool isAlreadyRegistFlag = false;
	UserPasswordRepository userPasswordRepository(_sqlApi);
	int ret = userPasswordRepository.isAlreadyRegist(userName, isAlreadyRegistFlag);
	if(ret != 0)
	{
		LOG_INFO("isAlreadyRegist failed! userName:%s", userName.c_str());
		code = DB_OP_FAILED;
		msg = "system error: insert data into DB failed!";
		return -1;
	}

	if(isAlreadyRegistFlag)
	{
		LOG_INFO("[%s] already regist!", userName.c_str());
		code = ALREADY_REGIST;
		string errmsg = userName + " already regist!";
		msg = errmsg;
		return 0;
	}

	//入库
	if(regist(userName, passWord) != 0)
	{
		code = DB_OP_FAILED;
		msg = "system error: insert data into DB failed!";
		return -1;
	}

	code = SUCCESS;
	msg = "regist success!";
	return 0;
}

bool Register::isLegalPassWordFormat(const string& passWord, string& errmsg)
{
	//解码密码
	char passWordDecoded[1024];
	base64_decode(passWord.c_str(), passWord.length(), passWordDecoded);
	LOG_INFO("passWordDecoded:%s", passWordDecoded);

	//检查长度
	int length = strlen(passWordDecoded);
	if(length < 6 or length > 20)
	{
		LOG_ERROR("length is < 6 or > 20! length:%d", length);
		errmsg = "pass word length is between 6 and 20.";
		return false;
	}

	//只能是字母、数字、下划线的组合
	regex reg("^[A-Za-z0-9_]+$");
	if(!regex_match(passWordDecoded, reg))
	{
		LOG_ERROR("passWord do not match ^[A-Za-z0-9_]+$");
		errmsg = "passWord do not match ^[A-Za-z0-9_]+$";
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
		return -1;
	}

	//加密
	clock_t before = clock();
	ret = bcrypt_hashpw(userName.c_str(), salt, passWordHash);
	if(ret != 0)
	{
		LOG_ERROR("bcrypt_hashpw failed!");
		return -1;
	}

	clock_t after = clock();
	LOG_INFO("bcrypt_hashpw time taken: %f seconds; passWordHash:%s", (float)(after - before) / CLOCKS_PER_SEC, passWordHash);

	//insert
	UserPassWordBO userPassWord;
	userPassWord.setUserName(userName);
	userPassWord.setPassWord(passWordHash);
	userPassWord.setSalt(salt);

	UserPasswordRepository userPasswordRepository(_sqlApi);
	ret = userPasswordRepository.insert(userPassWord);
	if(ret != 0)
	{
		return -1;
	}

	return 0;
}














