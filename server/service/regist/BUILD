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
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//server/service/base:loginManageServiceBase",
		"//server/repository/src:serverRespositorySrc",
		"//mylib/myLibEncrypt:myLibEncrypt",
	],
	visibility = ["//visibility:public"],
)