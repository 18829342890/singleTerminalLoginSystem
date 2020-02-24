load("@build_stack_rules_proto//cpp:cpp_grpc_library.bzl", "cpp_grpc_library")

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
		"-Iprotos/protoCode/",
		"-I@my_grpcpp_path",
		"-Imylib/thirdLib/",
	],
	deps = [
		"//protos/proto:message_receiver_cc_grpc",
		"@my_grpcpp//:grpcpp",
	],
	visibility = ["//client/main:__pkg__"],
)