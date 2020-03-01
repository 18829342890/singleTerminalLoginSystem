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
		"-Imylib/myLibEncrypt/",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//protos/proto:message_receiver_cc_grpc",
	],
	visibility = ["//visibility:public"],
)