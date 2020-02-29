cc_library(
	name = "syncClientInfo",
	srcs = [
		"syncClientInfo.cpp",
	],
	hdrs = [
		"syncClientInfo.h",
	],
	copts = [
		"-Imylib/enum/",
		"-Imylib/mylibLog/",
		"-Imylib/mylibSql/",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	visibility = ["//visibility:public"],
)