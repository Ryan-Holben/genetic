load("@rules_cc//cc:defs.bzl", "cc_library")

# Main build target to include full library
cc_library(
    name = "core",
    hdrs = glob(["include/**/*.h"]),
    # includes = ["include"],
    deps = [
        ":math",
        ":neural_network",
        ":platform",
        ":types",
    ],
    visibility = [
        "//datasets:__pkg__",
        "//evo:__pkg__",
        "//main:__pkg__",
    ],
    # copts = ["-O3",],
    # copts = ["-g",],
)

cc_library(
    name = "types",
    hdrs = ["include/core/types.h"],
    includes = ["include"],
)

cc_library(
    name = "neural_network",
    srcs = [
        "src/net_utils.cpp",
        "src/neural_network.cpp"
    ],
    hdrs = ["include/core/neural_network.h"],
    includes = ["include"],
    deps = [
        ":math",
        ":types",
    ],
)

cc_library(
    name = "math",
    srcs = ["src/math.cpp"],
    hdrs = ["include/core/math.h"],
    includes = ["include"],
    deps = [
        ":types",
    ],
)

cc_library(
    name = "platform",
    srcs = ["src/platform.cpp"],
    hdrs = ["include/core/platform.h"],
    includes = ["include"],
    deps = [
        "//vendor:backward-cpp",
    ]
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