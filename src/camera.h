#pragma once
#include <glm/glm.hpp>

class Camera {
protected:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::mat4 m_view_matrix;
    bool m_dirty { true };

public:
    Camera(glm::vec3 position, glm::vec3 target);

    glm::vec3 position() const { return m_position; }
    glm::vec3 target() const { return m_target; }

    void set_position(glm::vec3 position) { m_position = position; m_dirty = true; }
    void set_target(glm::vec3 target) { m_target = target; m_dirty = true; }

    glm::mat4 const& view_matrix();

};
