#pragma once

#include <string>
#include "server/service/base/loginManageServiceBase.h"
#include "server/infrastructure/include/rabbitmqClient.h"

using namespace std;
using namespace userLoginService::infrastructure;


class KickoutUser : public LoginManageServiceBase
{
public:
	KickoutUser(const sql::Connection* mysqlConnect, RabbitmqClient& rabbitmqClient, const string& exchange, const string& routeKey);
	virtual ~KickoutUser();
	int processkickout(const string& userName);

private:
	int noticeUserLogout(const string& userName, const string& clientUid);

private:
	RabbitmqClient& _rabbitmqClient;
	const string& _exchange;
    const string& _routeKey;
};