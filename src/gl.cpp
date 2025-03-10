#include <glad/glad.h>
#include <string>
#include <iostream>
#include <cassert>
#include "gl.h"
namespace Gl
{

static char const* gl_error_string(GLenum const err) noexcept
{
    switch (err)
    {
    // opengl 2 errors (8)
    case GL_NO_ERROR:
        return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

        // opengl 3 errors (1)
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

        // gles 2, 3 and gl 4 error are handled by the switch above
    default:
        assert(!"unknown error");
        return nullptr;
    }
}

void check_errors(std::string const& where)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "[" << where << "] OpenGL Error: " << gl_error_string(err) << std::endl;
    }
}

}
