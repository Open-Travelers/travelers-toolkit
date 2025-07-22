#pragma once
#include <glm/glm.hpp>

class Camera {
protected:
    glm::vec3 m_position { 0, 0, 0 };
    glm::vec3 m_target { 0, 0, 0 };

    float m_yaw { 0 };
    float m_pitch { 0 };

    bool m_dirty { true };

    glm::mat4 m_view_matrix;
    glm::vec3 m_front_vector { 0, 0, -1 };
    glm::vec3 m_right_vector { 1, 0, 0 };
    glm::vec3 m_up_vector { 0, 1, 0 };

public:
    Camera(glm::vec3 position, glm::vec3 target);

    void set_position(glm::vec3 position) { m_position = position; m_dirty = true; }
    void set_target(glm::vec3 target) { m_target = target; m_dirty = true; }

    void set_yaw(float value) { m_yaw = value; m_dirty = true; }
    void set_pitch(float value) { m_pitch = value; m_dirty = true; }

    void add_yaw(float angle) { m_yaw += angle; m_dirty = true; }
    void add_pitch(float angle) { m_pitch += angle; m_dirty = true; }
    void set_target_from_angles();

    void move(glm::vec3 delta) { set_position(m_position + delta); }

    glm::vec3 position() const { return m_position; }
    glm::vec3 target() const { return m_target; }

    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }
    glm::vec3 front() const { return m_front_vector; }
    glm::vec3 up() const { return m_up_vector; }
    glm::vec3 right() const { return m_right_vector; }

    glm::mat4 const& view_matrix();

};
