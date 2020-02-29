cc_library(
    name = "userLoginManager",
    srcs = ["userLoginManager.cpp"],
    hdrs = ["userLoginManager.h"],
	deps = [
		"//service/regist:register",
		"//service/login:loginer",
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
