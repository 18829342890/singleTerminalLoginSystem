
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
		"-Imylib/mylib_log/",
	],
	deps = [
		":commandLine",
		"//client/processCommandLine:processCommandLineLib",
		"//mylib/mylib_log:myLogLib"
	],

)
