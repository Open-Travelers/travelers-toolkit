#include "scene_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneRenderer::SceneRenderer()
{

}

SceneRenderer::~SceneRenderer()
{

}

bool SceneRenderer::load(Twoc::Nu::Scene const* scene)
{
    if (m_loaded)
        unload();

    // load shader
    if (!m_shader.load("shaders/vertex.glsl", "shaders/fragment.glsl"))
        return false;

    // load geometry
    int object_index = 0;
    for (auto &obj : scene->geometry_objects())
    {
        GLuint vbo, vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        int vertex_count = 0;
        for (auto &mesh : obj.meshes())
            vertex_count += mesh.vertices().size();

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Twoc::Nu::Vertex), NULL, GL_STATIC_DRAW);

        std::vector<int> mesh_vertex_offsets;
        int vertex_offset = 0;

        for (auto &mesh : obj.meshes())
        {
            vertex_count = mesh.vertices().size();
            glBufferSubData(GL_ARRAY_BUFFER, vertex_offset * sizeof(Twoc::Nu::Vertex), vertex_count * sizeof(Twoc::Nu::Vertex), mesh.vertices().data());
            mesh_vertex_offsets.push_back(vertex_offset);
            vertex_offset += vertex_count;
        }

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), 0);

        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4));

        // UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4+3*4));

        // color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4+3*4+2*4));

        RenderObject render_object;
        render_object.Vbo = vbo;
        render_object.Vao = vao;
        render_object.VertexOffsets = mesh_vertex_offsets;
        m_render_objects[object_index++] = render_object;
    }

    glBindVertexArray(0);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // load textures
    glEnable(GL_TEXTURE_2D);
    m_texture_ids.clear();
    m_texture_ids.reserve(scene->texture_count());
    for (int i = 0; i < scene->texture_count(); i++)
    {
        auto const& tex = scene->texture(i);
        GLuint id;
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.max_width(), tex.max_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bitmap(0).data().data());
        m_texture_ids.push_back(id);
    }

    // finish
    m_loaded = true;
    m_scene = scene;
    return true;
}


char const* gl_error_string(GLenum const err) noexcept
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


void SceneRenderer::render(glm::mat4 const& view, glm::mat4 const& projection)
{
    if (!m_scene || !m_loaded)
        return;

    glEnable(GL_TEXTURE_2D);
    m_shader.bind();

    GLuint mvp_loc = m_shader.get_uniform_location("u_mvp");
    GLuint diffuse_loc = m_shader.get_uniform_location("u_diffuse");
    GLuint ambient_loc = m_shader.get_uniform_location("u_ambient");
    GLuint ambient_power_loc = m_shader.get_uniform_location("u_ambient_power");
    GLuint backdrop_color_usage_loc = m_shader.get_uniform_location("u_backdrop_color_usage");
    GLuint alpha_loc = m_shader.get_uniform_location("u_alpha");

    glm::mat4 view_projection = projection * view * glm::scale(glm::mat4(1.0f), glm::vec3(-1, 1, 1));
    for (auto const& instance : m_scene->instances())
    {
        glm::mat4 mvp = view_projection * instance.transform_matrix();

        int object_id = instance.object_index();
        if (object_id < 0 || object_id >= m_scene->geometry_object_count())
            continue;

        RenderObject render_object = m_render_objects[object_id];
        glBindVertexArray(render_object.Vao);
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

        auto const& geometry_object = m_scene->geometry_object(object_id);
        int mesh_i = 0;
        for (auto const& mesh : geometry_object.meshes())
        {
            int material_index = mesh.material();
            Twoc::Nu::Material const& material = m_scene->material(material_index);
            glUniform3fv(ambient_loc, 1, material.data().ambient);
            glUniform3fv(diffuse_loc, 1, material.data().diffuse);
            glUniform1f(ambient_power_loc, material.power());
            glUniform1f(backdrop_color_usage_loc, 1.0f);
            glUniform1f(alpha_loc, material.alpha());
            if (material.texture_id() == -1)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            } else {
                glBindTexture(GL_TEXTURE_2D, m_texture_ids[material.texture_id()]);
            }

            for (auto const& prim : mesh.primitives())
            {
                GLenum mode = (prim.type() == Twoc::Nu::PrimitiveType::TriangleStrip) ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
                auto const& faces = prim.faces();
                auto const& indices = prim.indices();
                for (auto const& face : faces)
                {
                    glDrawElementsBaseVertex(mode, face.Count, GL_UNSIGNED_SHORT, indices.data() + face.Index, render_object.VertexOffsets[mesh_i]);
                    GLenum err;
                    while ((err = glGetError()) != GL_NO_ERROR)
                    {
                        std::cerr << "OpenGL Error: " << gl_error_string(err) << std::endl;
                    }
                }
            }
            mesh_i++;
        }

    }
}

void SceneRenderer::render_instance(size_t index, glm::mat4 const& view, glm::mat4 const& projection)
{

}

void SceneRenderer::unload()
{
    for (auto &id : m_texture_ids)
        glDeleteTextures(1, &id);

    for (auto &obj : m_render_objects)
    {
        glDeleteBuffers(1, &obj.second.Vbo);
    }
    m_render_objects.clear();
}
