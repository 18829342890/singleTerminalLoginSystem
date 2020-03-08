cc_library(
	name = "processCommandLineLib",
	hdrs = [
		"ProcessCommandLineBase.h",
		"Help.h",
		"Exiter.h",
		"Regist.h",
		"Login.h",
		"Logout.h",
	],
	copts = [
		"-Imylib/myLibEncrypt/",
		"-Imylib/enum/",
	],
	deps = [
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//mylib/enum:myLibEnum",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],
	visibility = ["//visibility:public"],
)