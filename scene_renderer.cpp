#include <iostream>
#include "glad/glad.h"
#include "scene_renderer.h"

static const GLchar* vertex_shader_src = \
        "#version 330 core\n"
        "layout ( location=0 ) in vec3 l_position;\n"
        "layout ( location=1 ) in vec3 l_normal;\n"
        "layout ( location=2 ) in vec2 l_uv;\n"
        "layout ( location=3 ) in uint l_color;\n"
        "uniform mat4 u_mvp;\n"
        "out vec4 v_color;\n"
        "out vec2 v_uv;\n"
        "void main() {\n"
        "    float r, g, b;"
        "    r = ((l_color) & uint(0xff)) / 256.0;\n"
        "    g = ((l_color >> 8) & uint(0xff)) / 256.0;\n"
        "    b = ((l_color >> 16) & uint(0xff)) / 256.0;\n"
        "    v_color = vec4(r, g, b, 1.0);\n"
        "    v_uv = l_uv;\n"
        "    gl_Position = u_mvp * vec4(l_position, 1.0f);\n"
        "}\n";
static const GLchar* fragment_shader_src = \
        "#version 330 core\n"
        "out vec4 color;\n"
        "in vec4 v_color;\n"
        "in vec2 v_uv;\n"
        "uniform vec3 u_diffuse;\n"
        "uniform vec3 u_ambient;\n"
        "uniform float u_ambient_power;\n"
        "uniform sampler2D u_texture;\n"
        "void main() {\n"
        "    vec3 ambien = v_color.xyz;\n"
        "    color = texture(u_texture, v_uv) * vec4(ambien, 1.0);\n"
        "}\n";

GeometryDefinitionRenderState::GeometryDefinitionRenderState(GLuint vao, GLuint vbo, std::vector<int> vertex_offsets, std::vector<int> texture_ids) 
    : m_vao(vao), m_vbo(vbo), m_vertex_offsets(vertex_offsets), m_texture_ids(texture_ids)
{
    
}

NuSceneRenderer::NuSceneRenderer()
{

}
NuSceneRenderer::~NuSceneRenderer()
{
    if (m_scene_loaded)
        deload();
        
}
void NuSceneRenderer::deload()
{
    for (auto& s : m_render_states)
    {
        glDeleteBuffers(1, &s.second.m_vbo);
        for (GLuint id : s.second.m_texture_ids)
            glDeleteTextures(1, &id);
        s.second.m_texture_ids.clear();
        glDeleteVertexArrays(1, &s.second.m_vao);
    }
    m_render_states.clear();

    glDeleteVertexArrays(1, &m_bounds_vao);
    glDeleteBuffers(1, &m_bounds_ibo);
    glDeleteBuffers(1, &m_bounds_vbo);
    
    m_bounds_vertices.clear();
    m_bounds_elements.clear();
}
bool NuSceneRenderer::load(Nu::Scene &scene)
{
    if (!m_shader_loaded)
    {
        m_shader = std::make_unique<Shader>(vertex_shader_src, fragment_shader_src);
        if (!m_shader.get()->initialize())
        {
            std::cerr << "Could not initialize shader, aborting." << std::endl;
            return false;
        }
        m_shader_loaded = true;
    }

    if (m_scene_loaded)
        deload();
    glEnable(GL_TEXTURE_2D);
    // create VBO and stick every single vertex of the same type inside
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // calculate N of vertices
    int defcount = scene.get_definition_count();
    for (int defi = 0; defi < defcount; defi++)
    {
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        Nu::GeometryDefinition* def = (Nu::GeometryDefinition*) scene.get_definitions_raw() + defi;
        int partcount = def->get_parts_count();

        int total_vertices = 0;
        for (int parti = 0; parti < partcount; parti++)
        {
            Nu::GeometryPart* part = (Nu::GeometryPart*) def->get_parts_raw() + parti;
            int vtxcount = part->get_vertex_count();
            total_vertices += vtxcount;
        }

        // generate and allocate VBO for object but don't copy
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(Nu::Vertex), NULL, GL_STATIC_DRAW);

        // copy data into the new VBO and store vertex offsets
        int vertex_offset = 0;
        std::vector<int> part_vertex_offsets;
        std::vector<int> part_texture_ids;

        std::map<int, GLint> texture_map;
        for (int parti = 0; parti < partcount; parti++)
        {
            Nu::GeometryPart* part = (Nu::GeometryPart*) def->get_parts_raw() + parti;
            int idx = part->material_index();
            
            Nu::Material *mat = (Nu::Material*) (scene.get_materials_raw() + idx);
            int tidx = mat->texture();
            if (!texture_map.contains(tidx))
            {
                Nu::Material *mat = (Nu::Material*) (scene.get_materials_raw() + idx);
                GLuint tid = 0;
                if (mat->texture() == -1)
                {
                    tid = 0;
                } else {
                    Nu::Texture tex = scene.get_textures()[mat->texture()];
                    if (tex.width() == 0 || tex.height() == 0 || tex.get_rgba_data() == nullptr)
                    {
                        tid = 0;
                    } else {
                        glGenTextures(1, &tid);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, tid);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        //std::cout << std::to_string(tex.get_rgba_data()[0]) << std::endl;
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.get_rgba_data());
                    }
                }
                texture_map[tidx] = tid;

                part_texture_ids.push_back(tid);
            } else {
                part_texture_ids.push_back(texture_map[tidx]);
            }
                
            
            int vtxcount = part->get_vertex_count();
            glBufferSubData(GL_ARRAY_BUFFER, vertex_offset * sizeof(Nu::Vertex), vtxcount * sizeof(Nu::Vertex), part->get_vertices_raw());
            part_vertex_offsets.push_back(vertex_offset);
            vertex_offset += vtxcount;
        }
        
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), 0);

        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4));

        // UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4+3*4));

        // color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4+3*4+2*4));

        // data per object: vao, vbo, vertex offsets
        m_render_states[defi] = GeometryDefinitionRenderState(vao, vbo, part_vertex_offsets, part_texture_ids);
    }

    glBindVertexArray(0);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Vec3 min = scene.min_bounds();
    Vec3 max = scene.max_bounds();
    

    m_bounds_vertices.reserve(8);
    m_bounds_vertices.clear();
    m_bounds_vertices.push_back(Nu::Vertex(Vec3(-0.5, -0.5, -0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3( 0.5, -0.5, -0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3( 0.5,  0.5, -0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3(-0.5,  0.5, -0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3(-0.5, -0.5,  0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3( 0.5, -0.5,  0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3( 0.5,  0.5,  0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    m_bounds_vertices.push_back(Nu::Vertex(Vec3(-0.5,  0.5,  0.5), Vec3(0, 0, 0), Vec2(0, 0), 0xff000000));
    
    m_bounds_elements.reserve(16);
    m_bounds_elements.clear();
    m_bounds_elements.push_back(0);
    m_bounds_elements.push_back(1);
    m_bounds_elements.push_back(2);
    m_bounds_elements.push_back(3);
    m_bounds_elements.push_back(4);
    m_bounds_elements.push_back(5);
    m_bounds_elements.push_back(6);
    m_bounds_elements.push_back(7);
    m_bounds_elements.push_back(0);
    m_bounds_elements.push_back(4);
    m_bounds_elements.push_back(1);
    m_bounds_elements.push_back(5);
    m_bounds_elements.push_back(2);
    m_bounds_elements.push_back(6);
    m_bounds_elements.push_back(3);
    m_bounds_elements.push_back(7);

    glGenBuffers(1, &m_bounds_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_bounds_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Nu::Vertex) * m_bounds_vertices.size(), m_bounds_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_bounds_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bounds_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * m_bounds_elements.size(), m_bounds_elements.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_bounds_vao);
    glBindVertexArray(m_bounds_vao);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), 0);

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4));

    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4+3*4));

    // color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Nu::Vertex), (void*)(3*4+3*4+2*4));

    
    glBindVertexArray(0);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_scene_loaded = true;
    return true;
}

bool NuSceneRenderer::render_scene_all(Nu::Scene &scene, Mat4x4 view, Mat4x4 projection)
{
    if (!m_scene_loaded || !m_shader_loaded)
        return false;
    glEnable(GL_TEXTURE_2D);
    m_shader->bind();
    GLuint mvp_loc = m_shader->get_uniform_location("u_mvp");
    GLuint diffuse_loc = m_shader->get_uniform_location("u_diffuse");
    GLuint ambient_loc = m_shader->get_uniform_location("u_ambient");
    GLuint ambient_power_loc = m_shader->get_uniform_location("u_ambient_power");
    
    
    for (Nu::Instance instance : scene.get_instances())
    {
        Mat4x4 transform = instance.get_transform_matrix();
        Mat4x4 mvp = projection * view * Mat4x4::Scale(Vec3(-1, 1, 1)) * transform;

        GeometryDefinitionRenderState state = m_render_states[instance.get_geometry_object()];
        glBindVertexArray(state.m_vao);
        

        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, mvp.data());

        // most big brain fucking way of doing this. accomplishes nothing but makes code look worse
        int object_id = instance.get_geometry_object();
        //std::cout << "ID: " << std::to_string(object_id) << std::endl;
        auto const& definitions = scene.get_definitions();
        if (object_id >= definitions.size() || object_id < 0)
            continue;

        Nu::GeometryDefinition definition = scene.get_definitions().at(object_id);
        int which_offset = 0;
        for (Nu::GeometryPart part : definition.get_parts())
        {
            int mat_idx = part.material_index();
            Nu::Material* mt = (Nu::Material*) (scene.get_materials_raw() + mat_idx);
            glUniform3fv(ambient_loc, 1, mt->ambient().data());
            glUniform3fv(diffuse_loc, 1, mt->diffuse().data());
            glUniform1f(ambient_power_loc, mt->power());

            glBindTexture(GL_TEXTURE_2D, state.m_texture_ids[which_offset]);
            for (Nu::GeometryPrimitive prim : part.get_primitives())
            {
                GLenum mode = (prim.get_type() == Nu::GeometryPrimitive::Type::tPrimTriStrip) ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
                
                for (int i = 0; i < prim.get_faces_count(); i++)
                {
                    Nu::GeometryPrimitiveFace *face = prim.get_faces_raw() + i;
                    glDrawElementsBaseVertex(mode, face->count, GL_UNSIGNED_SHORT, prim.get_data_raw() + face->data_offset, state.m_vertex_offsets[which_offset]);
                }
            }
            which_offset++;
        }
    }
    /*
    Vec3 size = scene.max_bounds() - scene.min_bounds();
    Vec3 center = (scene.min_bounds() + scene.max_bounds()) / 2.0f;
    Mat4x4 mvp = projection * view * Mat4x4::Scale(Vec3(-1, 1, 1)) * Mat4x4::Scale(size);

    glBindVertexArray(m_bounds_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_bounds_vbo);
    GLuint mvp_loc = m_shader->get_uniform_location("u_mvp");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, mvp.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bounds_ibo);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*) (4 * sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*) (8 * sizeof(GLushort)));
*/
    
    return true;
}
