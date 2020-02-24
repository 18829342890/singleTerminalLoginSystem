
cc_library(
    name = "commandLine",
    srcs = ["commandLine.cpp"],
    hdrs = ["commandLine.h"],
    copts = [
		"-Iclient/processCommandLine/",
		"-Imylib/thirdLib/"
	],
	deps = [
		"//client/processCommandLine:processCommandLineLib",
	]
)

cc_binary (
	name = "client",
	srcs = ["main.cpp"],
	defines = ["BAZEL_BUILD"],
	copts = [
		"-I@my_grpcpp_path",
		"-Iclient/processCommandLine/",
	],
	deps = [
		":commandLine",
		"//client/processCommandLine:processCommandLineLib",
		"//protos/proto:message_receiver_cc_grpc",
		"@com_github_grpc_grpc//:grpc++",
	],

)
