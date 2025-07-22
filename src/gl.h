#pragma once
#include <string>

#define GL_CHECK(name, ...) \
    do { \
        name(__VA_ARGS__); \
        Gl::check_errors(#name); \
    } while (0)
    
namespace Gl {
    void check_errors(std::string const& where);
}
