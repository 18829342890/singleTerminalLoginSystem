cc_library(
	name = "serverRespository",
	srcs = [
		"../src/userPasswordRepository.cpp",
		"../src/userLoginManageRepository.cpp",
	],
	hdrs = [
		"userPasswordRepository.h",
		"userLoginManageRepository.h",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Iserver/domain",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/domain:serverDomain",
	],
	visibility = ["//visibility:public"],
)