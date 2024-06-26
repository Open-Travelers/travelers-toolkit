#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H
#include <memory>
#include "nu/scene.h"
#include "glad/glad.h"
#include "shader.h"
#include <map>
class GeometryDefinitionRenderState {
protected:
    GLuint m_vao, m_vbo;
    std::vector<int> m_vertex_offsets;
    std::vector<int> m_texture_ids;
public:
    GeometryDefinitionRenderState(GLuint vao, GLuint vbo, std::vector<int> vertex_offsets, std::vector<int> texture_ids);
    GeometryDefinitionRenderState() {}
    friend class NuSceneRenderer;
};

class NuSceneRenderer {
protected:
    bool m_scene_loaded { false };
    bool m_shader_loaded { false };
    std::map<int, GeometryDefinitionRenderState> m_render_states;
    std::unique_ptr<Shader> m_shader;
    
    std::vector<Nu::Vertex> m_bounds_vertices;
    std::vector<uint16_t> m_bounds_elements;
    GLuint m_bounds_vao, m_bounds_vbo, m_bounds_ibo;

    void deload();
public:
    NuSceneRenderer();
    ~NuSceneRenderer();
    bool load(Nu::Scene &scene);
    bool render_scene_instance(Nu::Scene &scene, int id);
    bool render_scene_all(Nu::Scene &scene, Mat4x4 view, Mat4x4 projection);
};

#endif