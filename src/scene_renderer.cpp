#include "scene_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl.h"

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
    Gl::check_errors("ShaderLoad");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // load textures
    Gl::check_errors("glEnable");
    m_texture_ids.clear();
    m_texture_ids.reserve(scene->texture_count());
    for (int i = 0; i < scene->texture_count(); i++)
    {
        auto const& tex = scene->texture(i);
        GLuint id;
        glGenTextures(1, &id);
        Gl::check_errors("glGenTextures");
        glActiveTexture(GL_TEXTURE0);
        Gl::check_errors("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, id);
        Gl::check_errors("glBindTexture");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        Gl::check_errors("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        Gl::check_errors("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        Gl::check_errors("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Gl::check_errors("glTexParameteri");

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.max_width(), tex.max_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bitmap(0).data().data());
        Gl::check_errors("glTexImage2D");
        m_texture_ids.push_back(id);
    }

    // load geometry
    int object_index = 0;
    for (auto &obj : scene->geometry_objects())
    {
        GLuint vbo, vao;
        glGenVertexArrays(1, &vao);
        Gl::check_errors("glGenVertexArrays");
        glBindVertexArray(vao);
        Gl::check_errors("glBindVertexArray");

        int vertex_count = 0;
        for (auto &mesh : obj.meshes())
            vertex_count += mesh.vertices().size();

        glGenBuffers(1, &vbo);
        Gl::check_errors("glGenBuffers");
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        Gl::check_errors("glBindBuffer");
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Twoc::Nu::Vertex), nullptr, GL_STATIC_DRAW);
        Gl::check_errors("glBindBuffer");

        std::vector<int> mesh_vertex_offsets;
        int vertex_offset = 0;

        for (auto &mesh : obj.meshes())
        {
            auto const& vertices = mesh.vertices();
            glBufferSubData(GL_ARRAY_BUFFER, vertex_offset * sizeof(Twoc::Nu::Vertex), vertices.size() * sizeof(Twoc::Nu::Vertex), vertices.data());
            Gl::check_errors("glBufferSubData");
            mesh_vertex_offsets.push_back(vertex_offset);
            vertex_offset += vertices.size();
        }

        // position
        glEnableVertexAttribArray(0);
        Gl::check_errors("glEnableVertexAttribArray");
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), 0);
        Gl::check_errors("glVertexAttribPointer");
        // normal
        glEnableVertexAttribArray(1);
        Gl::check_errors("glEnableVertexAttribArray");
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4));
        Gl::check_errors("glVertexAttribPointer");

        // UV
        glEnableVertexAttribArray(2);
        Gl::check_errors("glEnableVertexAttribArray");
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4+3*4));
        Gl::check_errors("glVertexAttribPointer");

        // color
        glEnableVertexAttribArray(3);
        Gl::check_errors("glEnableVertexAttribArray");
        glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*)(3*4+3*4+2*4));
        Gl::check_errors("glVertexAttribPointer");

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

    // finish
    m_loaded = true;
    m_scene = scene;
    return true;
}


void SceneRenderer::render(glm::mat4 const& view, glm::mat4 const& projection)
{
    if (!m_scene || !m_loaded)
        return;

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
        Gl::check_errors("glBindVertexArray");
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
        Gl::check_errors("glUniformMatrix4fv");

        auto const& geometry_object = m_scene->geometry_object(object_id);
        int mesh_i = 0;
        for (auto const& mesh : geometry_object.meshes())
        {
            int material_index = mesh.material();
            Twoc::Nu::Material const& material = m_scene->material(material_index);
            glUniform3f(ambient_loc, material.ambient(0), material.ambient(1), material.ambient(2));
            Gl::check_errors("glUniform3f");
            //glUniform3fv(diffuse_loc, 1, material.data().diffuse);
            //Gl::check_errors("glUniform3fv");
            glUniform1f(ambient_power_loc, material.power());
            Gl::check_errors("glUniform1f");
            glUniform1f(backdrop_color_usage_loc, 1.0f);
            Gl::check_errors("glUniform1f");
            glUniform1f(alpha_loc, material.alpha());
            Gl::check_errors("glUniform1f");

            if (material.texture_id() == -1)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
                Gl::check_errors("glBindTexture");
            } else {
                glBindTexture(GL_TEXTURE_2D, m_texture_ids[material.texture_id()]);
                Gl::check_errors("glBindTexture");
            }

            for (auto const& prim : mesh.primitives())
            {
                GLenum mode = (prim.type() == Twoc::Nu::PrimitiveType::TriangleStrip) ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
                auto const& faces = prim.faces();
                auto const& indices = prim.indices();
                for (auto const& face : faces)
                {
                    glDrawElementsBaseVertex(mode, face.Count, GL_UNSIGNED_SHORT, indices.data() + face.Index, render_object.VertexOffsets[mesh_i]);
                    Gl::check_errors("glDrawElementsBaseVertex");
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
