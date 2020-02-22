cc_library(
    name = "messageReceiverImpl",
    srcs = ["messageReceiver.cpp"],
    hdrs = ["messageReceiver.h"],
)

cc_library(
    name = "myLogLib",
    srcs = ["logrecord.c"],
    hdrs = ["logrecord.h"],
)

cc_binary (
	name = "messageReceiver",
	srcs = ["main.cpp"],
	deps = [
		":messageReceiverImpl",
		":myLogLib",
	],
)