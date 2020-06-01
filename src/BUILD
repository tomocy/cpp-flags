load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flags",
    srcs = ["flags.cc"],
    hdrs = ["flags.h"],
    visibility = [
        "//tests:__pkg__",
    ],
    deps = [
        "//src:analysis",
    ],
)

cc_library(
    name = "analysis",
    srcs = ["analysis.cc"],
    hdrs = ["analysis.h"],
    visibility = [
        "//tests:__pkg__",
    ],
)
