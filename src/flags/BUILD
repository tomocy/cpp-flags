load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flags",
    srcs = ["flags.cc"],
    hdrs = ["flags.h"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/flags:exceptions",
        "//src/flags:flag",
        "//src/flags:values",
        "//src/flags/analysis",
    ],
)

cc_library(
    name = "flag",
    srcs = ["flag.cc"],
    hdrs = ["flag.h"],
    deps = [
        "//src/flags:exceptions",
        "//src/flags:values",
    ],
)

cc_library(
    name = "values",
    srcs = ["values.cc"],
    hdrs = ["values.h"],
    deps = [
        "//src/flags:exceptions",
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
