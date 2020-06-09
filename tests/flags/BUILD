load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "flag_set",
    srcs = ["flag_set_test.cc"],
    deps = [
        "//src/flags",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "flag",
    srcs = ["flag_test.cc"],
    deps = [
        "//src/flags",
        "@gtest//:gtest_main",
    ],
)
