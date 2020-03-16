cc_library(
	name = "register",
	srcs = [
		"register.cpp",
	],
	hdrs = [
		"register.h",
	],
	copts = [
		"-Imylib/enum",
		"-Imylib/mylibLog",
		"-Imylib/myLibEncrypt",
		"-Iserver/service/base",
		"-Iserver/repository/include",
		"-Iserver/infrastructure/include",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//server/service/base:loginManageServiceBase",
		"//server/repository/src:serverRespositorySrc",
		"//server/infrastructure/src:userLoginInfoCacheSrc",
		"//mylib/myLibEncrypt:myLibEncrypt",
	],
	visibility = ["//visibility:public"],
)