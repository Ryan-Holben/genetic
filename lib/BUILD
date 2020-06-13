load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "example",
    srcs = ["src/example.cpp"],
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//main:__pkg__"],
)

cc_test (
    name = "example-test",
    srcs = [
        "test/unit-tests.cpp",
    ],
    deps = [
        ":example",
        "@gtest//:gtest",
        "@gtest//:gtest_main", # Only if hello_test.cc has no main()
    ],
)