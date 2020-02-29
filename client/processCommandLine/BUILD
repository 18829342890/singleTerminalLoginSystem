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
	],
	deps = [
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//mylib/enum:myLibEnum",
	],
	linkopts = [
		"-L/usr/local/lib64",
		"-lssl",
		"-lcrypto",
	],
	visibility = ["//client/main:__pkg__"],
)