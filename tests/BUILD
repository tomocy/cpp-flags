load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "flags",
    srcs = ["flags_test.cc"],
    deps = [
        "//src:flags",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "analysis",
    srcs = ["analysis_test.cc"],
    deps = [
        "//src:analysis",
        "@gtest//:gtest_main",
    ],
)
