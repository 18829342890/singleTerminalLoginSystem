cc_library(
	name = "syncClientInfo",
	srcs = [
		"syncClientInfo.cpp",
	],
	hdrs = [
		"syncClientInfo.h",
	],
	copts = [
		"-Imylib/enum",
		"-Imylib/mylibLog",
		"-Imylib/mylibSql",
		"-Iserver/repository/include",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
		"//server/repository/include:serverRespository",
	],
	visibility = ["//visibility:public"],
)