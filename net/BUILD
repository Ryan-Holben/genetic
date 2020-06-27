load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "net",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//evo:__pkg__"],
    deps = ["//core"],
)

cc_test (
    name = "test-net",
    srcs = [
        "test/test-net.cpp",
    ],
    deps = [
        ":net",
        "@gtest//:gtest",
        "@gtest//:gtest_main", # Only if hello_test.cc has no main()
    ],
)

cc_binary(
    name = "benchmark",
    srcs = ["bench/bench-net.cpp"],
    deps = [":net", "//bench:benchlib"],
)