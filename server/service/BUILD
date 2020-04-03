cc_library(
    name = "userLoginManageService",
    srcs = ["userLoginManageService.cpp"],
    hdrs = ["userLoginManageService.h"],
    copts = [
    	"-Imylib/mylibLog",
    	"-Iservice/base",
    	"-Iservice/login",
    	"-Iservice/regist",
		"-Iservice/logout",
		"-Iservice/kickout",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/service/base:loginManageServiceBase",
		"//server/service/login:loginer",
		"//server/service/regist:register",
		"//server/service/logout:logout",
		"//server/service/kickout:kickout",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],
	visibility = ["//visibility:public"],
)