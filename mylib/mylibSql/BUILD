cc_library(
    name = "mysqlLib",
    srcs = ["/usr/local/mysql/lib/libmysqlclient.so"],
    hdrs = glob(["/usr/local/mysql/include/*.h"]),
)

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
		":mysqlLib"
	],
	visibility = ["//visibility:public"],
)