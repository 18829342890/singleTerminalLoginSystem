

cc_binary (
	name = "client",
	srcs = ["main.cpp"],
	copts = [
		"-Imylib/mylibLog/",
		"-Iclient/commanLine/processCommandLine/",
		"-Iclient/service/receiveLogoutNotice/receiveMessage/",
		"-Iclient/myToolLib",
	],
	deps = [
		"//client/commanLine:commandLine",
		"//client/receiveMessage:receiveMessage",
		"//client/service/receiveLogoutNotice/receiveMessage:receiveLogoutNotice",
		"//client/myToolLib:myTool",
		"//mylib/mylibLog:myLibLog",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],

)
