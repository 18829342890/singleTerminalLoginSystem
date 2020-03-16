cc_library(
	name = "userLoginInfoCacheSrc",
	srcs = [
		"userLoginCache.cpp",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Iserver/domain",
		"-Iserver/infrastructure/include",
	],
	linkopts = [
		"-L/usr/local/redis/deps/hiredis",
		"-lhiredis",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/domain:serverDomain",
		"//server/infrastructure/include:userLoginInfoCache"
	],
	visibility = ["//visibility:public"],
)