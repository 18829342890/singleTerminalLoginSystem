cc_library(
	name = "myLibEncrypt",
	hdrs = [
		"encrypt.h",
		"md5.h"
	],
	srcs = [
		"md5.cc",
	],
	linkopts = [
		"-L/usr/local/lib64",
		"-lssl",
		"-lcrypto",
	],
	visibility = ["//visibility:public"],
)