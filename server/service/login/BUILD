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
		"-Iprotos/protoCode",
		"-Iserver/repository/include",

	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//mylib/enum:myLibEnum",
		"//mylib/mylibSql:myLibSqlApi",
		"//mylib/myLibEncrypt:myLibEncrypt",
		"//server/repository/include:serverRespository",
		"//protos/protoCode:message_receiver_cc_grpc",
	],
	visibility = ["//visibility:public"],
)