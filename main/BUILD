load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        "//core",
        "//datasets:xor",
        "//evo",
    ],
    # copts = ["-g -O0"],
)