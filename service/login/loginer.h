#ifndef __LOGINER_H__
#define __LOGINER_H__

#include "sqlApi.h"
#include <string>



class Loginer
{
public:
	Loginer(SqlApi& sqlApi);
	virtual ~Loginer();
	int processLogin(const string& userName, const string& passWord, int& code, string& msg);
	int processLogout(const string& userName, int logoutType, int& code, string& msg);

private:
	//用户名和密码是否有效
	int isValidUserNameAndPassWord(const string& userName, const string& passWord, bool& isValid);
	//该用户是否已登录
	int getIsAlreadyLogined(const string& userName, bool& isAlreadyLogined);
	//登录
	int login(const string& userName, const string& passWord);
	//通知已登录的设备退出登录
	int noticeLoginedClientLogout(const string& userName);
	//获取已登录设备对应的fd
	int getAlreadyLoginedClientInfo(const string& userName, string& ip, int& port);
	//通知设备退出登录
	int noticeLogout(const string& clientIp, int clientPort);


private:
	SqlApi _sqlApi;
};






#endif