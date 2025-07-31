#include "spline.h"

namespace Twoc::Nu {

Spline::Spline(size_t name_index, std::vector<glm::vec3> points) : m_name_index(name_index), m_points(points)
{
}

}
