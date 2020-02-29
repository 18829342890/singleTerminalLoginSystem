cc_library(
	name = "register",
	srcs = [
		"register.cpp",
	],
	hdrs = [
		"register.h",
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
	visibility = ["//visibility:public"],
)