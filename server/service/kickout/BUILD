cc_library(
	name = "kickout",
	srcs = [
		"kickoutUser.cpp",
	],
	hdrs = [
		"kickoutUser.h",
	],
	copts = [
		"-Imylib/enum/",
		"-Imylib/mylibLog/",
		"-Iserver/service/base",
		"-Iserver/repository/include",
		"-Iserver/infrastructure/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//server/service/base:loginManageServiceBase",
		"//server/repository/src:serverRespositorySrc",
		"//server/infrastructure/src:rabbitmqClientSrc",
	],
	visibility = ["//visibility:public"],
)