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
		"-Imylib/mylibSql",
		"-Imylib/myLibEncrypt",
		"-Iserver/repository/include",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
		"//server/repository/include:serverRespository",
		"//mylib/myLibEncrypt:myLibEncrypt",
	],
	visibility = ["//visibility:public"],
)