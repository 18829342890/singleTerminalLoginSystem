
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
		"-Iclient/processCommandLine/",
	],
	deps = [
		":commandLine",
		"//client/receiveMessage:receiveMessage",
		"//mylib/mylibLog:myLibLog",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],

)
