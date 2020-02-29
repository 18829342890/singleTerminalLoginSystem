cc_library(
	name = "myLibSqlApi",
	srcs = [
		"sqlApi.cc",
	],
	hdrs = [
		"sqlApi.h",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	visibility = ["//visibility:public"],
)