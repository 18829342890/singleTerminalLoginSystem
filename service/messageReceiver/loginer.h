#ifndef __LOGINER_H__
#define __LOGINER_H__

#include "sqlApi.h"
#include <string>



class Loginer
{
public:
	Loginer(SqlApi& sqlApi);
	virtual ~Loginer();
	int processLogin(const string& userName, const string& passWord, int sendToClientFd, int& code, string& msg);

private:
	//用户名和密码是否有效
	int isValidUserNameAndPassWord(const string& userName, const string& passWord, bool& isValid);
	//该用户是否已登录
	bool isAlreadyLogined(const string& userName);
	//登录
	int login(const string& userName, const string& passWord, int sendToClientFd);
	//获取已登录设备对应的fd
	int getAlreadyLoginedClientFd(const string& userName, int& sendToClientFd, unsigned long long& fuiId);
	//通知设备退出登录
	int noticeLogout(int fd);
	//
	int setLogoutStatusThenLogin(unsigned long long fuiId, const string& userName, const string& passWord, int sendToClientFd);


private:
	SqlApi _sqlApi;
};






#endif