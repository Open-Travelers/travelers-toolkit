#include <iostream>
#include <sstream>
#include <fstream>
#include "shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{
    unload();
}

void Shader::unload()
{
    if (m_shader_program != 0)
        glDeleteProgram(m_shader_program);
}

void Shader::bind()
{
    if (m_shader_program != 0)
        return;
    glUseProgram(m_shader_program);
}

static std::string collect_shader(std::ifstream &file, bool &error) {
    std::stringstream stream;
    int line_no = 1;
    for (std::string line; std::getline(file, line);) {
        if (line.rfind("//!", 0) == 0) {
            line = line.erase(0, 3);
            if (line.rfind("include ", 0) == 0) {
                line = line.erase(0, 8);
                line_no++;

                std::ifstream new_file(line);
                if (!new_file.is_open()) {
                    stream.str("");
                    stream << "Could not open file: '" << line << "'!" << std::endl;
                    error = true;
                    return stream.str();
                }
                stream << collect_shader(new_file, error) << std::endl;
                stream << "#line " << line_no << std::endl;
            }
        } else {
            stream << line << std::endl;
            line_no++;
        }
    }
    return stream.str();
}

bool Shader::load(std::string const& vertex_shader_path, std::string const& fragment_shader_path)
{
    if (m_shader_program != 0)
    {
        std::cerr << "Shader already loaded!" << std::endl;
        return true;
    }

    // collect vertex shader from file
    std::ifstream vtx_stream(vertex_shader_path);
    if (!vtx_stream.is_open())
    {
        std::cerr << "Vertex shader file '" << vertex_shader_path << "' doesn't exist!" << std::endl;
        return false;
    }

    bool vtx_error = false;
    std::string vtx_source = collect_shader(vtx_stream, vtx_error);
    if (vtx_error)
    {
        std::cerr << "Failed to collect vertex shader: " << vtx_source << "!" << std::endl;
        return false;
    }

    // collect fragment shader from file
    std::ifstream frag_stream(fragment_shader_path);
    if (!frag_stream.is_open())
    {
        std::cerr << "Fragment shader file '" << fragment_shader_path << "' doesn't exist!" << std::endl;
        return false;
    }

    bool frag_error = false;
    std::string frag_source = collect_shader(frag_stream, frag_error);
    if (frag_error)
    {
        std::cerr << "Failed to collect fragment shader: " << frag_source << "!" << std::endl;
        return false;
    }

    // create vertex shader
    const char *vtx_raw = vtx_source.c_str();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vtx_raw, nullptr);
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
    const char* frag_raw = frag_source.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_raw, NULL);
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
