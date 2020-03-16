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