cc_library(
	name = "receiveLogoutNotice",
	srcs = [
		"receiveLogoutNotice.cpp",
	],
	hdrs = [
		"receiveLogoutNotice.h",
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