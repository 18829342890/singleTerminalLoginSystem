
cc_library(
    name = "commandLine",
    srcs = ["commandLine.cpp"],
    hdrs = ["commandLine.h"],
    copts = [
		"-Iclient/processCommandLine/",
	],
	deps = [
		"//client/processCommandLine:processCommandLineLib",

	]
)

cc_binary (
	name = "client",
	srcs = ["main.cpp"],
	copts = [
		"-Imylib/mylibLog/",
		"-Iclient/receiveMessage/",
	],
	deps = [
		":commandLine",
		"//mylib/mylibLog:myLibLog",
		"//client/receiveMessage:receiveMessage",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],

)
