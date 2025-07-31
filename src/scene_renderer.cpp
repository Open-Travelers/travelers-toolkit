#include "scene_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl.h"
#include "imgui.h"

SceneRenderer::SceneRenderer()
{

}

SceneRenderer::~SceneRenderer()
{

}

std::unique_ptr<RenderableScene> SceneRenderer::load_scene(Twoc::Nu::Scene const* scene)
{

    return {};
}

bool SceneRenderer::load(Twoc::Nu::Scene const* scene)
{
    if (m_loaded)
        unload();

    // load regular shader
    if (!m_shader.load("shaders/vertex.glsl", "shaders/fragment.glsl"))
        return false;
    Gl::check_errors("ShaderLoad");

    if (!m_shader.load("shaders/instanced_vertex.glsl", "shaders/instanced_fragment.glsl"))
        return false;
    Gl::check_errors("ShaderLoad");

    GLuint vbo;
    GL_CHECK(glGenBuffers, 1, &vbo);
    GL_CHECK(glBindBuffer, GL_ARRAY_BUFFER, vbo);

    // load textures
    m_texture_ids.clear();
    for (int i = 0; i < scene->texture_count(); i++)
    {
        auto const& tex = scene->texture(i);
        GLuint id;
        GL_CHECK(glGenTextures, 1, &id);
        GL_CHECK(glBindTexture, GL_TEXTURE_2D, id);
        GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GL_CHECK(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL_CHECK(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA, tex.max_width(), tex.max_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bitmap(0).raw_data());
        GL_CHECK(glGenerateMipmap, GL_TEXTURE_2D);
        m_texture_ids.push_back(id);
    }

    // load geometry
    int object_index = 0;
    for (auto &obj : scene->geometry_objects())
    {
        GLuint vbo, vao;
        GL_CHECK(glGenVertexArrays, 1, &vao);
        GL_CHECK(glBindVertexArray, vao);

        int vertex_count = 0;
        for (auto &mesh : obj.meshes())
            vertex_count += mesh.vertices().size();

        GL_CHECK(glGenBuffers, 1, &vbo);
        GL_CHECK(glBindBuffer, GL_ARRAY_BUFFER, vbo);
        GL_CHECK(glBufferData, GL_ARRAY_BUFFER, vertex_count * sizeof(Twoc::Nu::Vertex), nullptr, GL_STATIC_DRAW);
        
        std::vector<int> mesh_vertex_offsets;
        int vertex_offset = 0;

        for (auto &mesh : obj.meshes())
        {
            auto const& vertices = mesh.vertices();
            GL_CHECK(glBufferSubData, GL_ARRAY_BUFFER, vertex_offset * sizeof(Twoc::Nu::Vertex), vertices.size() * sizeof(Twoc::Nu::Vertex), vertices.data());
            mesh_vertex_offsets.push_back(vertex_offset);
            vertex_offset += vertices.size();
        }

        // position
        GL_CHECK(glEnableVertexAttribArray, 0);
        GL_CHECK(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*) Twoc::Nu::Vertex::PositionOffset);

        // normal
        GL_CHECK(glEnableVertexAttribArray, 1);
        GL_CHECK(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*) Twoc::Nu::Vertex::NormalOffset);

        // UV
        GL_CHECK(glEnableVertexAttribArray, 2);
        GL_CHECK(glVertexAttribPointer, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*) Twoc::Nu::Vertex::UvOffset);

        // color
        GL_CHECK(glEnableVertexAttribArray, 3);
        GL_CHECK(glVertexAttribPointer, 3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Twoc::Nu::Vertex), (void*) Twoc::Nu::Vertex::ColorOffset);

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

static float Transform_Flip[16] = {
-1, 0, 0, 0, 
0, 1, 0, 0, 
0, 0, 1, 0, 
0, 0, 0, 1
};
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
    GLuint texture_loc = m_shader.get_uniform_location("u_texture");

    for (auto const& instance : m_scene->instances())
    {
        int object_id = instance.object_index();
        if (object_id < 0 || object_id >= m_scene->geometry_object_count())
            continue;

        RenderObject render_object = m_render_objects[object_id];
        GL_CHECK(glBindVertexArray, render_object.Vao);
        
        auto const& geometry_object = m_scene->geometry_object(object_id);
        glm::mat4 transform = instance.transform_matrix() * glm::translate(glm::mat4(1), geometry_object.origin());
        glm::mat4 flipper = glm::make_mat4(Transform_Flip);
        glm::mat4 mvp = projection * view * flipper * transform;

        int mesh_i = 0;
        for (auto const& mesh : geometry_object.meshes())
        {
            int material_index = mesh.material();
            Twoc::Nu::Material const& material = m_scene->material(material_index);

            GL_CHECK(glActiveTexture, GL_TEXTURE0);
            GL_CHECK(glUniformMatrix4fv, mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
            GL_CHECK(glUniform3f, ambient_loc, material.ambient(0), material.ambient(1), material.ambient(2));
            GL_CHECK(glUniform3fv, diffuse_loc, 1, material.data().diffuse);
            GL_CHECK(glUniform1f, ambient_power_loc, material.power());
            GL_CHECK(glUniform1f, backdrop_color_usage_loc, 0.0f);
            GL_CHECK(glUniform1f, alpha_loc, material.alpha());
            GL_CHECK(glUniform1i, texture_loc, 0);

            if (material.texture_id() == -1 || material.texture_id() > m_texture_ids.size())
            {
                GL_CHECK(glBindTexture, GL_TEXTURE_2D, 0);
            } else {
                GL_CHECK(glBindTexture, GL_TEXTURE_2D, m_texture_ids[material.texture_id()]);
            }

            for (auto const& prim : mesh.primitives())
            {
                auto mode = (prim.type() == Twoc::Nu::PrimitiveType::TriangleStrip) ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
                auto const& faces = prim.faces();
                auto const& indices = prim.indices();
                for (auto const& face : faces)
                {
                    GL_CHECK(glDrawElementsBaseVertex, mode, face.Count, GL_UNSIGNED_SHORT, indices.data() + face.Index, render_object.VertexOffsets[mesh_i]);
                }
            }
            mesh_i++;
        }
    }
    /*
    if (ImGui::Begin("Weird"))
    {
        for (int row = 0; row < 4; row++)
        {
            ImGui::PushID(row);
            ImGui::InputFloat4("##Matrix", Transform_Flip + row * 4);
            ImGui::PopID();
        }
    }
    ImGui::End();*/
    
}

void SceneRenderer::render_instances(size_t index, glm::mat4 const& view, glm::mat4 const& projection, std::vector<glm::mat4> matrices)
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
