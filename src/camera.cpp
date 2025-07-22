#include "camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

glm::mat4 const& Camera::view_matrix()
{
    if (m_dirty)
    {
        m_view_matrix = glm::lookAt(m_position, m_target, glm::vec3 { 0, 1, 0 });

        m_right_vector = glm::vec3(m_view_matrix[0][0], m_view_matrix[1][0], m_view_matrix[2][0]);
        m_up_vector = glm::vec3(m_view_matrix[0][1], m_view_matrix[1][1], m_view_matrix[2][1]);
        m_front_vector = glm::vec3(m_view_matrix[0][2], m_view_matrix[1][2], m_view_matrix[2][2]);
        m_dirty = false;
    }

    return m_view_matrix;
}

void Camera::set_target_from_angles()
{
    glm::vec3 front_angle { 0};
    front_angle.x = cosf(m_yaw) * cosf(m_pitch);
    front_angle.y = sinf(m_pitch);
    front_angle.z = sinf(m_yaw) * cosf(m_pitch);
    m_target = m_position + glm::normalize(front_angle) * 2.f;
    m_dirty = true;
}

Camera::Camera(glm::vec3 position, glm::vec3 target) : m_position(position), m_target(target)
{

}
