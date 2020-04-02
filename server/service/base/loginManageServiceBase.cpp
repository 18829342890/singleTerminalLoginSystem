#include <sstream>
#include "loginManageServiceBase.h"
#include "mylib/enum/code.h"
#include "mylib/cJSON/cJSON.h"
#include "server/repository/include/userLoginManageRepository.h"

using namespace userLoginSystem::myEnum;
using namespace userLoginService::repository;

LoginManageServiceBase::LoginManageServiceBase(const sql::Connection* mysqlConnect)
	:_code(SUCCESS),
	 _msg("success"),
	 _mysqlConnect(mysqlConnect)
{}


LoginManageServiceBase::~LoginManageServiceBase()
{}


int LoginManageServiceBase::getCode()
{
	return _code;
}

string LoginManageServiceBase::getMsg()
{
	return _msg;
}

string LoginManageServiceBase::getNoticeLogoutMsg(const string& userName, const string& clientUid, int msgType)
{
	//创建json
	cJSON* root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "userName", userName.c_str());
	cJSON_AddStringToObject(root, "clinetUid", clientUid.c_str());
	cJSON_AddNumberToObject(root, "messageType", msgType);

	//转化为string
	std::stringstream ss;
	ss << cJSON_PrintUnformatted(root);

	cJSON_Delete(root);

	return ss.str();
}