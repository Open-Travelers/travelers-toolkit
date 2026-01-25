#pragma once

#include <glad/glad.h>
#include <map>
#include <memory>
#include <glm/glm.hpp>

#include "twoc/nu/scene.h"
#include "shader.h"

class RenderableScene {
protected:

public:
    RenderableScene();
    ~RenderableScene();


};

struct SplineVertex {
    static constexpr size_t PositionOffset = 0;
    static constexpr size_t ColorOffset = sizeof(glm::vec3);

    glm::vec3 position;
    glm::vec3 color;
};

class SceneRenderer {
private:
    Shader m_shader;
    Shader m_spline_shader;

    std::vector<GLuint> m_texture_ids;
    bool m_loaded { false };
    Twoc::Nu::Scene const* m_scene;

    struct RenderObject {
        GLuint Vao;
        GLuint Vbo;
        std::vector<size_t> VertexOffsets;
    };

    struct SplineRenderObject {
        GLuint Vao;
        GLuint Vbo;
        GLsizei VertexCount;
    };
    std::map<int, RenderObject> m_render_objects;
    std::vector<SplineRenderObject> m_spline_render_objects;

public:
    SceneRenderer();
    ~SceneRenderer();

    std::unique_ptr<RenderableScene> load_scene(Twoc::Nu::Scene const* scene);

    bool load(Twoc::Nu::Scene const* scene);
    void unload();

    GLuint get_texture(int id) {
        if (id < 0 || id >= m_texture_ids.size())
            return -1;
        return m_texture_ids[id];
    }

    void render(glm::mat4 const& view, glm::mat4 const& projection);
    void render_instances(size_t index, glm::mat4 const& view, glm::mat4 const& projection, std::vector<glm::mat4> matrices);
};