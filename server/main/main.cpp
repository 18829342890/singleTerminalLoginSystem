#include "userLoginManageService.h"
#include "simpleIni.h"
#include <errno.h>
#include <fstream>
#include <sstream>



std::string readFile2String(std::string file_name) {
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

	LOG_INFO("serverAddress:%s, saltWorkFactor:%d", serverAddress, saltWorkFactor);
	LOG_INFO("mysqlHost:%s, mysqlUser:%s, mysqlPassWord:%s, mysqlDb:%s, mysqlPort:%d", mysqlHost, mysqlUser, mysqlPassWord, mysqlDb, mysqlPort);

	//连接mysql
	SqlApi sqlApi(mysqlHost, mysqlUser, mysqlPassWord, mysqlDb, mysqlPort);
	int ret = sqlApi.connectMysql();
	if(ret < 0)
	{
		LOG_ERROR("connectMysql failed!");
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

	UserLoginManageService service(sqlApi, saltWorkFactor);
	ServerBuilder builder;
	builder.AddListeningPort(serverAddress, grpc::SslServerCredentials(sslOps));
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	LOG_INFO("Server listening on %s", serverAddress);

  	server->Wait();
}