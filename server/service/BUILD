cc_library(
    name = "userLoginManageService",
    srcs = ["userLoginManageService.cpp"],
    hdrs = ["userLoginManageService.h"],
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
		"//server/service/login:loginer",
		"//server/service/regist:register",
		"//server/service/syncClientInfo:syncClientInfo",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],
	visibility = ["//visibility:public"],
)