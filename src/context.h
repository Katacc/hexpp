#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>

struct Context {
    std::string preset = "default"; // Default preset
    std::string config = "Debug"; // Default config
    const std::string BUILD_DIR = "build";

    std::string CXX_COMPILER_PATH = "g++";
    std::string C_COMPILER_PATH = "gcc";
    std::string C_DEBUGGER_PATH = "gdb";
    std::string MAKE_PATH = "make";
};

#endif // CONTEXT_H