cc_binary (
	name = "client",
	srcs = ["main.cpp"],
	copts = [
		"-Imylib/mylibLog/",
		"-Iclient/commanLine/processCommandLine/",
		"-Iclient/myToolLib",
	],
	deps = [
		"//client/commanLine:commandLine",
		"//client/myToolLib:myTool",
		"//mylib/mylibLog:myLibLog",
		"//mylib/myLibIni:myLibIni",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],

)
