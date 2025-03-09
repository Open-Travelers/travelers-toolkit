#ifndef SHADER_H
#define SHADER_H

extern "C" {
#include <glad/glad.h>
}

#include <string>

class Shader {
protected:
    GLuint m_shader_program {0};
public:
    Shader();
    ~Shader();
    bool load(std::string const& vertex_shader_path, std::string const& fragment_shader_path);
    void unload();

    void bind();
    GLuint get_shader_program();
    GLuint get_uniform_location(const char *name);
};

#endif
