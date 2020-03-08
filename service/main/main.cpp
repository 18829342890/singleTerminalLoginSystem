#include "userLoginManager.h"
#include <errno.h>
#include <fstream>
#include <sstream>


static const char* SERVICE_ADDRESS = "0.0.0.0:50051";

std::string readFile2String(std::string file_name) {
  std::ifstream ifs(file_name);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return buffer.str();
}


int main()
{
	SqlApi sqlApi;
	int ret = sqlApi.connectMysql();
	if(ret < 0)
	{
		LOG_ERROR("connectMysql failed!");
		return -1;
	}

	std::string key = readFile2String("../../auth/server.key");
  	std::string cert = readFile2String("../../auth/server.crt");
  	std::string root = readFile2String("../../auth/ca.crt");
  	grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert = {key,cert};
  	grpc::SslServerCredentialsOptions sslOps;
  	sslOps.pem_root_certs = root;
  	sslOps.pem_key_cert_pairs.push_back (key_cert);

	MessageReceiver service(sqlApi);
	ServerBuilder builder;
	builder.AddListeningPort(SERVICE_ADDRESS, grpc::SslServerCredentials(sslOps));
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	LOG_INFO("Server listening on %s", SERVICE_ADDRESS);

  	server->Wait();
}