cc_library(
	name = "loginer",
	srcs = [
		"loginer.cpp",
	],
	hdrs = [
		"loginer.h",
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
		"//protos/proto:message_receiver_cc_grpc",
	],
	linkopts = [
		"-L/usr/local/mysql/lib",
		"-lmysqlclient",
	],
	visibility = ["//visibility:public"],
)