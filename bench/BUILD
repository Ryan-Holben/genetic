load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")


py_binary(
    name = "bench",
    srcs = ["scripts/bench.py"],
)

cc_library(
    name = "benchlib",
    srcs = [],
    # hdrs = glob(["include/**/*.h"]),
    hdrs = ["include/bench/benc.h"],
    includes = ["include"],
    deps = [],
    visibility = ["//visibility:public"]
)