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
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//server/service/base:loginManageServiceBase",
		"//server/repository/src:serverRespositorySrc",
	],
	visibility = ["//visibility:public"],
)