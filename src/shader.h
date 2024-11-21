#ifndef SHADER_H
#define SHADER_H

extern "C" {
#include <glad/glad.h>
}

class Shader {
protected:
    GLuint m_shader_program {0};
    bool m_initialized { false };
    const GLchar* m_vertex_source;
    const GLchar* m_fragment_source;
public:
    Shader(const GLchar *vtx_source, const GLchar* frag_source);
    ~Shader();
    bool initialize();
    void bind();
    GLuint get_shader_program();
    GLuint get_uniform_location(const char *name);
};

#endif