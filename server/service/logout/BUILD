cc_library(
	name = "logout",
	srcs = [
		"logout.cpp",
	],
	hdrs = [
		"logout.h",
	],
	copts = [
		"-Imylib/enum",
		"-Imylib/mylibLog",
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
	],
	visibility = ["//visibility:public"],
)