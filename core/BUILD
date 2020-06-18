load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "core",
    srcs = ["src/core.cpp"],
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//main:__pkg__"],
)

cc_test (
    name = "neural-network-core",
    srcs = [
        "test/unit-tests.cpp",
    ],
    deps = [
        ":core",
        "@gtest//:gtest",
        "@gtest//:gtest_main", # Only if hello_test.cc has no main()
    ],
)