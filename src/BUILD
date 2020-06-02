load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flags",
    srcs = ["flags.cc"],
    hdrs = ["flags.h"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src:analysis",
        "//src:exceptions",
        "//src:values",
    ],
)

cc_library(
    name = "values",
    srcs = ["values.cc"],
    hdrs = ["values.h"],
    deps = [
        "//src:exceptions",
    ],
)

cc_library(
    name = "exceptions",
    srcs = ["exceptions.cc"],
    hdrs = ["exceptions.h"],
    visibility = [
        "//tests:__pkg__",
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
