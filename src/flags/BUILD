load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flags",
    hdrs = ["flags.h"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//src/flags:exceptions",
        "//src/flags:flag",
        "//src/flags:flag_set",
        "//src/flags:parser",
        "//src/flags:values",
        "//src/flags/analysis",
    ],
)

cc_library(
    name = "flag_set",
    srcs = ["flag_set.cc"],
    hdrs = ["flag_set.h"],
    deps = [
        "//src/flags:exceptions",
        "//src/flags:flag",
        "//src/flags:parser",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        "//src/flags:exceptions",
        "//src/flags:flag",
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
