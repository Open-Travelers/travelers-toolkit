#include "shader.h"
#include <iostream>

Shader::Shader(const GLchar *vtx_source, const GLchar* frag_source)
{
    m_vertex_source = vtx_source;
    m_fragment_source = frag_source;
}
Shader::~Shader()
{
    if (m_initialized)
        glDeleteProgram(m_shader_program);
}
void Shader::bind()
{
    if (!m_initialized)
        return;
    glUseProgram(m_shader_program);
}
bool Shader::initialize()
{
    // create vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &m_vertex_source, nullptr);
    glCompileShader(vertex_shader);

    // check for compilation errors
    int success = 0;
    char log[512] = {0};

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, log);
        std::cerr << "Failed to compile vertex shader: " << std::string(log) << std::endl;
        return false;
    }
    success = 0;

    // create fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &m_fragment_source, NULL);
    glCompileShader(fragment_shader);

    // check for compilation errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cerr << "Failed to compile fragment shader: " << std::string(log) << std::endl;

        glDeleteShader(vertex_shader);
        return false;
    }

    // create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // check for linking errors
    success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, log);
        std::cerr << "Failed to link shader program: " << std::string(log) << std::endl;

        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteProgram(shader_program);
        return false;

    }

    // delete shaders and return success
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    m_shader_program = shader_program;
    m_initialized = true;
    return true;
}

GLuint Shader::get_shader_program() 
{ 
    return m_shader_program; 
}

GLuint Shader::get_uniform_location(const char *name)
{
    return glGetUniformLocation(m_shader_program, name);
}