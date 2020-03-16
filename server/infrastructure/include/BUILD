cc_library(
	name = "userLoginInfoCache",
	hdrs = [
		"userLoginCache.h",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Iserver/domain",
	],
	linkopts = [
		"-L/usr/local/redis/deps/hiredis",
		"-lhiredis",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/domain:serverDomain",

	],
	visibility = ["//visibility:public"],
)