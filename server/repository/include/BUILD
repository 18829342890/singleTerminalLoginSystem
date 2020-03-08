cc_library(
	name = "serverRespository",
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