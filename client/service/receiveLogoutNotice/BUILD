cc_library(
	name = "receiveMessage",
	srcs = [
		"receiveMessage.cpp",
	],
	hdrs = [
		"receiveMessage.h",
	],
	copts = [
		"-Imylib/mylibLog/",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//protos/proto:message_receiver_cc_grpc",
	],
	visibility = ["//client/main:__pkg__"],
)