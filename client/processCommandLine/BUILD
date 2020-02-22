cc_library(
	name = "processCommandLineLib",
	hdrs = [
		"ProcessCommandLineBase.h",
		"Help.h",
		"Exiter.h",
		"Regist.h",
		"Login.h",
		"Logout.h",
	],
	copts = [
		"-Iprotos/protoCode/"
	],
	deps = [
		"//protos/proto:person_cc_proto"
	],
	visibility = ["//client/main:__pkg__"],
)