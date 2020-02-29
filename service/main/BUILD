cc_library(
    name = "userLoginManager",
    srcs = ["userLoginManager.cpp"],
    hdrs = ["userLoginManager.h"],
    copts = [
    	"-Iservice/login/",
    	"-Iservice/regist/",
		"-Iservice/syncClientInfo/",
		"-Imylib/enum/",
		"-Imylib/mylibLog/",
		"-Imylib/mylibSql/",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
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
