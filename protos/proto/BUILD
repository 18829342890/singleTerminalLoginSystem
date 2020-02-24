load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_java//java:defs.bzl", "java_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@build_stack_rules_proto//cpp:cpp_grpc_library.bzl", "cpp_grpc_library")




proto_library(
    name = "message_receiver_proto",
    srcs = ["messageReceiver.proto"],
)

cc_proto_library(
    name = "message_receiver_cc_proto",
    deps = [":message_receiver_proto"],
    visibility = ["//visibility:public"],
)

cpp_grpc_library(
    name = "message_receiver_cc_grpc",
    srcs = [":message_receiver_proto"],
    grpc_only = True,
    deps = [":message_receiver_cc_proto"],
    visibility = ["//visibility:public"],
)
