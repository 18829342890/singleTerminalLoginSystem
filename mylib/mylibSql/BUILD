cc_library(
	name = "myLibSqlApi",
	srcs = [
		"sqlApi.cc",
	],
	hdrs = [
		"sqlApi.h",
	],
	copts = [
		"-I/usr/local/mysql/include",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	visibility = ["//visibility:public"],
)