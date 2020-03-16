cc_library(
	name = "loginer",
	srcs = [
		"loginer.cpp",
	],
	hdrs = [
		"loginer.h",
	],
	copts = [
		"-Imylib/enum/",
		"-Imylib/mylibLog/",
		"-Imylib/myLibEncrypt/",
		"-Iserver/repository/include",
		"-Iserver/infrastructure/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//server/repository/src:serverRespositorySrc",
		"//server/infrastructure/src:userLoginInfoCacheSrc",
	],
	visibility = ["//visibility:public"],
)