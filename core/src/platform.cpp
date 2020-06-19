#include <core/platform.h>

#include <iostream>

// #include <execinfo.h>  // for backtrace
// #include <dlfcn.h>     // for dladdr
// #include <cxxabi.h>    // for __cxa_demangle

#include <backward-cpp/backward.hpp>
#include <string.h>
using namespace backward;

namespace core {

// // This function produces a stack backtrace with demangled function & method names.
// void Backtrace(int skip = 0)
// {
// 	void *callstack[128];
// 	const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
// 	char buf[1024];
// 	int nFrames = backtrace(callstack, nMaxFrames);
// 	char **symbols = backtrace_symbols(callstack, nFrames);

// 	// std::ostringstream trace_buf;
// 	for (int i = skip; i < nFrames; i++) {
// 		Dl_info info;
// 		if (dladdr(callstack[i], &info)) {
// 			char *demangled = NULL;
// 			int status;
// 			demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
// 			snprintf(buf, sizeof(buf), "%-3d %0*p \x1b[0;31m%s\x1b[0m + %zd\n",
// 					 i, 2 + sizeof(void*) * 2, callstack[i],
// 					 status == 0 ? demangled : info.dli_sname,
// 					 (char *)callstack[i] - (char *)info.dli_saddr);
// 			free(demangled);
// 		} else {
// 			snprintf(buf, sizeof(buf), "%-3d %*0p\n",
// 					 i, 2 + sizeof(void*) * 2, callstack[i]);
// 		}
// 		// trace_buf << buf;
// 		std::cout << buf;

// 		snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
// 		// trace_buf << buf;
// 		// std::cout << buf;
// 	}
// 	free(symbols);
// 	if (nFrames == nMaxFrames) {
// 		// trace_buf << "[truncated]\n";
// 		std::cout << "[truncated]\n";
// 	}
// 	// return trace_buf.str();
// }

#define TO_STR(x) #x

void Borktrace(int sig, size_t level, const char* file, int line, const char* function) {
    std::cout << "\n";
    std::cout << COL_RED << "\tSignal:\t\t" << COL_NONE << strsignal(sig) << "\n";
    std::cout << COL_RED << "\tFunction:\t" << COL_NONE << function << "\n";
    std::cout << COL_RED << "\tFile:\t\t" << COL_NONE << file << "\n";
    std::cout << COL_RED << "\tLine #:\t\t" << COL_NONE << line << "\n";
    std::cout << "\n";

    StackTrace stackTrace;
    TraceResolver resolver;
    stackTrace.load_here();
    resolver.load_stacktrace(stackTrace);

    std::cout << "----- " << COL_YELLOW << "Stacktrace" << COL_NONE << " -----\n";
    size_t i = 0;
    for (; i < level; i++) {
        const ResolvedTrace trace = resolver.resolve(stackTrace[i]);
        std::cout << "(" << COL_GREEN_BOLD << i << COL_NONE << " - " << COL_BLUE
                  << trace.object_function << COL_NONE << ")\n";
    }
    if (level > 0) {
        std::cout << "----- " << COL_YELLOW << "Traceback and exit starts here" << COL_NONE
                  << " -----\n";
    }
    for (; i < stackTrace.size(); i++) {
        const ResolvedTrace trace = resolver.resolve(stackTrace[i]);
        std::cout << " " << COL_GREEN_BOLD << i << COL_NONE << " - " << COL_BLUE
                  << trace.object_function << COL_NONE << "\n";
    }
    std::cout << "----- " << COL_YELLOW << "end stacktrace" << COL_NONE << " -----\n";
    exit(1);
}

void BacktraceHandler(int sig) { Borktrace(sig); }

void InstallBacktraceHandler() {
    for (size_t i = 0; i < NSIG; i++) {
        signal(i, BacktraceHandler);
    }
}

} // namespace core