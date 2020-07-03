load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "xor",
    hdrs = ["include/datasets/xor.h"],
    includes = ["include"],
    deps = ["//core:core"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "mnist",
    hdrs = ["include/datasets/mnist.h"],
    data = [":data/train-images-idx3-ubyte", ":data/train-labels-idx1-ubyte"],
    includes = ["include"],
    deps = ["//core:core"],
    visibility = ["//main:__pkg__"],
)