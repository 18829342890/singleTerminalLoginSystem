cc_library(
	name = "rabbitmqClient",
	hdrs = [
		"rabbitmqClient.h",
	],
	copts = [
		"-Imylib/mylibLog",
	],
	linkopts = [
		"-L/usr/local/Cellar/rabbitmq-c/0.10.0/include",
		"-lrabbitmq",
	],
	deps = [
		"//mylib/mylibLog:myLibLog",
	],
	visibility = ["//visibility:public"],
)