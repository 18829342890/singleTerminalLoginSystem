cc_library(
	name = "heartBeat",
	srcs = [
		"heartBeat.cpp",
	],
	hdrs = [
		"heartBeat.h",
	],
	copts = [
		"-Imylib/enum/",
		"-Imylib/mylibLog/",
		"-Iserver/repository/include",
		"-Iserver/infrastructure/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//server/repository/src:serverRespositorySrc",
		"//server/infrastructure/src:userLoginInfoCacheSrc",
	],
	visibility = ["//visibility:public"],
)