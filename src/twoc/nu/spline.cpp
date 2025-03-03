#include "spline.h"

namespace Twoc::Nu {

Spline::Spline(const std::string &name, std::vector<glm::vec3> points) : m_name(name), m_points(points)
{
}

}
