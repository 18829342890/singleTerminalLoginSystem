cc_library(
    name = "userLoginManager",
    srcs = ["userLoginManager.cpp"],
    hdrs = ["userLoginManager.h"],
    copts = [
    	"-Imylib/mylibLog/",
    	"-Imylib/mylibSql/",
    	"-Iservice/login/",
    	"-Iservice/regist/",
		"-Iservice/syncClientInfo/",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/mylibSql:myLibSqlApi",
		"//service/login:loginer",
		"//service/regist:register",
		"//service/syncClientInfo:syncClientInfo",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	]
)

cc_binary (
	name = "userLoginManagerService",
	srcs = ["main.cpp"],
	deps = [
		":userLoginManager",
	],

)
