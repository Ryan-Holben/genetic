#pragma once

#include <assert.h>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdexcept>

#include <backward-cpp/backward.hpp>
using namespace backward;

namespace core {

// https://bixense.com/clicolors/
constexpr char* COL_NONE = "\x1b[0m";
constexpr char* COL_RED = "\x1b[0;31m";
constexpr char* COL_RED_BOLD = "\x1b[1;31m";
constexpr char* COL_GREEN = "\x1b[0;32m";
constexpr char* COL_GREEN_BOLD = "\x1b[1;32m";
constexpr char* COL_YELLOW = "\x1b[0;33m";
constexpr char* COL_YELLOW_BOLD = "\x1b[1;33m";
constexpr char* COL_BLUE = "\x1b[0;34m";
constexpr char* COL_BLUE_BOLD = "\x1b[1;34m";
constexpr char* COL_PINK = "\x1b[0;35m";
constexpr char* COL_PINK_BOLD = "\x1b[1;35m";

void InstallBacktraceHandler();
void Borktrace(int sig = -1, size_t level = 0, const char* file = "", int line = -1,
               const char* function = "");

// Asserts that conditional is true, exits with debugging info otherwise.
#define ASSERT(conditional)                                                                        \
    if (!(conditional)) {                                                                          \
        std::cout << COL_RED_BOLD << "ASSERT FAILED:" << COL_NONE << " ";                          \
        std::cout << COL_PINK_BOLD << #conditional << COL_NONE;                                    \
        Borktrace(SIGINT, 3, __FILE__, __LINE__, __PRETTY_FUNCTION__);                             \
    }

// Asserts that conditional is true, exits with debugging info and the supplied message otherwise.
#define ASSERT_WITH_MSG(conditional, message)                                                      \
    if (!(conditional)) {                                                                          \
        std::cout << COL_RED_BOLD << "ASSERT FAILED:" << COL_NONE << " ";                          \
        std::cout << COL_PINK_BOLD << #conditional << COL_NONE << "\n";                            \
        std::cout << "\t" << COL_RED << "Message:\t" << COL_NONE << message;                       \
        Borktrace(SIGINT, 3, __FILE__, __LINE__, __PRETTY_FUNCTION__);                             \
    }

#define OUTPUT_PARAM(pointer)                                                                      \
    ASSERT_WITH_MSG(pointer != nullptr, "Provided output parameter was a nullptr")

} // namespace core