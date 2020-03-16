cc_library(
	name = "loginManageServiceBase",
	srcs = [
		"loginManageServiceBase.cpp",
	],
	hdrs = [
		"loginManageServiceBase.h",
	],
	copts = [
		"-Imylib/mylibLog/",
		"-Iserver/repository/include",
		"-Iserver/infrastructure/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/repository/src:serverRespositorySrc",
		"//server/infrastructure/src:userLoginInfoCacheSrc",
	],
	visibility = ["//visibility:public"],
)