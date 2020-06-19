load("@rules_cc//cc:defs.bzl", "cc_library")

# backward-cpp include is used for getting stacktraces
cc_library(
    name = "backward-cpp",
    srcs = ["src/backward.cpp"],
    hdrs = ["include/backward-cpp/backward.hpp"],
    includes = ["include"],
    visibility = [
        "//core:__pkg__",
    ],
    # copts = ["-O3",],
    # copts = ["-g",],
)