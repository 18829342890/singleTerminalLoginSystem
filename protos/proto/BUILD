load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_java//java:defs.bzl", "java_library")
load("@rules_proto//proto:defs.bzl", "proto_library")


cc_proto_library(
    name = "person_cc_proto",
    deps = [":person_proto"],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "person_proto",
    srcs = ["messageReceiver.proto"],
    deps = [
        "@com_google_protobuf//:any_proto",
    ],
)
