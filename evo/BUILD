load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "evo",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//main:__pkg__"],
    deps = ["//core", "//net"],
)

cc_test (
    name = "test-evo",
    srcs = [
        "test/unit-tests.cpp",
    ],
    deps = [
        ":evo",
        "@gtest//:gtest",
        "@gtest//:gtest_main", # Only if hello_test.cc has no main()
    ],
)

cc_binary(
    name = "benchmark",
    srcs = ["bench/bench-evo.cpp"],
    deps = [":evo", "//bench:benchlib"],
)