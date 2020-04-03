cc_library(
	name = "rabbitmqClientSrc",
	srcs = [
		"rabbitmqClient.cpp",
	],
	copts = [
		"-Imylib/mylibLog",
		"-Iserver/infrastructure/include",
	],
	linkopts = [
		"-L/usr/local/Cellar/rabbitmq-c/0.10.0/include",
		"-lrabbitmq",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
		"//server/infrastructure/include:rabbitmqClient",
	],
	visibility = ["//visibility:public"],
)