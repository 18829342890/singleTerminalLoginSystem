#include "userLoginManager.h"
#include <errno.h>


static const char* SERVICE_ADDRESS = "127.0.0.1:8080";


int main()
{
	SqlApi sqlApi;
	int ret = sqlApi.connectMysql();
	if(ret < 0)
	{
		LOG_ERROR("connectMysql failed!");
		return -1;
	}

	MessageReceiver service(sqlApi);
	ServerBuilder builder;
	builder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);
	builder.AddListeningPort(SERVICE_ADDRESS, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	LOG_INFO("Server listening on %s", SERVICE_ADDRESS);

  	server->Wait();
}