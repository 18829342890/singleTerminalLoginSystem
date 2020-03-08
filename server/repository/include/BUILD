cc_library(
	name = "serverRespository",
	hdrs = [
		"userPasswordRepository.h",
		"userLoginManageRepository.h",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Imylib/mylibSql",
		"-Iserver/domain",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/mylibSql:myLibSqlApi",
		"//server/domain:serverDomain",

	],
	visibility = ["//visibility:public"],
)