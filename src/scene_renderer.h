#pragma once

#include <glad/glad.h>
#include "twoc/nu/scene.h"
#include "shader.h"
#include <map>
#include <glm/glm.hpp>

class SceneRenderer {
private:
    Shader m_shader;
    std::vector<GLuint> m_texture_ids;
    bool m_loaded { false };
    Twoc::Nu::Scene const* m_scene;

    struct RenderObject {
        GLuint Vao;
        GLuint Vbo;
        std::vector<int> VertexOffsets;
    };

    std::map<int, RenderObject> m_render_objects;

public:
    SceneRenderer();
    ~SceneRenderer();
    bool load(Twoc::Nu::Scene const* scene);
    void unload();

    GLuint get_texture(int id) {
        if (id < 0 || id >= m_texture_ids.size())
            return -1;
        return m_texture_ids[id];
    }

    void render(glm::mat4 const& view, glm::mat4 const& projection);
    void render_instance(size_t index, glm::mat4 const& view, glm::mat4 const& projection);
};
