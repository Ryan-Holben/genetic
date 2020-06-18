load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "xor",
    hdrs = ["include/datasets/xor.h"],
    includes = ["include"],
    deps = ["//core:core"],
    visibility = ["//main:__pkg__"],
)

# cc_test (
#     name = "neural-network-core",
#     srcs = [
#         "test/unit-tests.cpp",
#     ],
#     deps = [
#         ":datasets",
#         "@gtest//:gtest",
#         "@gtest//:gtest_main", # Only if hello_test.cc has no main()
#     ],
# )