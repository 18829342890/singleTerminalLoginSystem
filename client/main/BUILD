cc_library(
    name = "commandLine",
    srcs = ["commandLine.cpp"],
    hdrs = ["commandLine.h"],
    copts = [
		"-Iclient/processCommandLine/"
	],
	deps = [
		"//client/processCommandLine:processCommandLineLib",
	]
)

cc_binary (
	name = "client",
	srcs = ["main.cpp"],
	copts = [
		"-Iclient/processCommandLine/",
	],
	deps = [
		":commandLine",
		"//client/processCommandLine:processCommandLineLib",
		"//protos/proto:person_cc_proto"
	],

)
