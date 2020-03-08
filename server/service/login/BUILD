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
		"-Imylib/myLibIni",
		"-Iprotos/proto",
		"-Iserver/repository/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//server/repository/include:serverRespository",
		"//protos/proto:message_receiver_cc_grpc",
	],
	visibility = ["//visibility:public"],
)