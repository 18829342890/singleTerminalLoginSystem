cc_library(
	name = "myLibSqlApi",
	srcs = [
		"sqlApi.cc",
	],
	hdrs = [
		"sqlApi.h",
	],
	copts = [
		"-Imylib/mylibLog",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
	],
	visibility = ["//visibility:public"],
)