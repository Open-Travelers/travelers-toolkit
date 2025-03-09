#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 const& Camera::view_matrix()
{
    if (m_dirty)
    {
        m_view_matrix = glm::lookAt(m_position, m_target, glm::vec3 { 0, 1, 0 });
        m_dirty = false;
    }

    return m_view_matrix;
}

Camera::Camera(glm::vec3 position, glm::vec3 target) : m_position(position), m_target(target)
{

}
