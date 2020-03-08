cc_library(
	name = "myLibEncrypt",
	hdrs = [
		"base64.h",
		"bcrypt.h",
		"md5.h"
	],
	srcs = [
		"bcrypt.cc",
		"md5.cc",
	],
	linkopts = [
		"-L/usr/local/lib64",
		"-lssl",
		"-lcrypto",
		"-L/usr/local/lib",
		"-lbsd",
	],
	visibility = ["//visibility:public"],
)