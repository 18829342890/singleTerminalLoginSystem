#include <errno.h>
#include <fstream>
#include <sstream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <boost/scoped_ptr.hpp>
#include <hiredis/hiredis.h>
#include "userLoginManageService.h"
#include "simpleIni.h"
#include "rabbitmqClient.h"



static std::string readFile2String(std::string file_name) 
{
	std::ifstream ifs(file_name);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

int main()
{
	//读取配置
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("../config/server_config.ini");

	const char* serverAddress = ini.GetValue("server", "address", "localhost:50051");
	int saltWorkFactor = strtol(ini.GetValue("server", "saltWorkFactor", "12"), NULL, 10);

	const char* mysqlHost = ini.GetValue("mysql", "host", "localhost");
	const char* mysqlUser = ini.GetValue("mysql", "user", "root");
	const char* mysqlPassWord = ini.GetValue("mysql", "password", "root");
	const char* mysqlDb = ini.GetValue("mysql", "db", "user");
	int mysqlPort = strtol(ini.GetValue("mysql", "port", "3306"), NULL, 10);

	const char* rabbitmqHost = ini.GetValue("rabbitmq", "host", "127.0.0.1");
	int rabbitmqPort = strtol(ini.GetValue("rabbitmq", "port", "5672"), NULL, 10);
	const char* rabbitmqUser = ini.GetValue("rabbitmq", "user", "guest");
	const char* rabbitmqPassWord = ini.GetValue("rabbitmq", "password", "guest");
	const char* loginExchange = ini.GetValue("rabbitmq", "loginExchange", "loginExchange");
	const char* loginRoutekey = ini.GetValue("rabbitmq", "loginRoutekey", "loginRoutekey");
	const char* loginQueuename = ini.GetValue("rabbitmq", "loginQueuename", "loginQueue");

	LOG_INFO("serverAddress:%s, saltWorkFactor:%d", serverAddress, saltWorkFactor);
	LOG_INFO("mysqlHost:%s, mysqlUser:%s, mysqlPassWord:%s, mysqlDb:%s, mysqlPort:%d", mysqlHost, mysqlUser, mysqlPassWord, mysqlDb, mysqlPort);
	LOG_INFO("rabbitmqHost:%s, rabbitmqPort:%d, rabbitmqUser:%s, rabbitmqPassWord:%s", rabbitmqHost, rabbitmqPort, rabbitmqUser, rabbitmqPassWord);
	LOG_INFO("loginExchange:%s, loginRoutekey:%s, loginQueuename:%s", loginExchange, loginRoutekey, loginQueuename);

	//连接mysql
	sql::Driver *driver = sql::mysql::get_driver_instance();
	sql::Connection* mysqlConnect = driver->connect(mysqlHost, mysqlUser, mysqlPassWord);
	sql::Statement* stmt = mysqlConnect->createStatement();
	string execCmd = "use ";
	execCmd += mysqlDb;
	stmt->execute(execCmd);

	//连接rabbitmq
	RabbitmqClient rabbitmq;
	if(rabbitmq.connect(rabbitmqHost, rabbitmqPort, rabbitmqUser, rabbitmqPassWord) != 0)
	{
		LOG_ERROR("rabbitmq.connect failed!");
		return -1;
	}
	if(rabbitmq.exchangeDeclare(loginExchange, "direct") != 0)
	{
		LOG_ERROR("rabbitmq.exchangeDeclare failed!");
		return -1;
	}
	if(rabbitmq.queueDeclare(loginQueuename) != 0)
	{
		LOG_ERROR("rabbitmq.queueDeclare failed!");
		return -1;
	}
	if(rabbitmq.queueBind(loginQueuename, loginExchange, loginRoutekey) != 0)
	{
		LOG_ERROR("rabbitmq.queueBind failed!");
		return -1;
	}
	
	//创建grpc服务
	std::string key = readFile2String("../../auth/server.key");
  	std::string cert = readFile2String("../../auth/server.crt");
  	std::string root = readFile2String("../../auth/ca.crt");
  	grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert = {key,cert};
  	grpc::SslServerCredentialsOptions sslOps;
  	sslOps.pem_root_certs = root;
  	sslOps.pem_key_cert_pairs.push_back (key_cert);

	UserLoginManageService service(mysqlConnect, rabbitmq, loginExchange, loginRoutekey, loginQueuename, saltWorkFactor);
	ServerBuilder builder;
	builder.AddListeningPort(serverAddress, grpc::SslServerCredentials(sslOps));
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	LOG_INFO("Server listening on %s", serverAddress);

  	server->Wait();
  	stmt->close();
  	mysqlConnect->close();
  	rabbitmq.disconnect();
}