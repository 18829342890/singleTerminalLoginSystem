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
		"-Imylib/cJSON/",
		"-Iexternal/usr/local/include",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/cJSON:myLibJson",
	],
	visibility = ["//visibility:public"],
)