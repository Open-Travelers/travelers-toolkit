#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace Twoc::Nu {

class Spline {
protected:
    std::string m_name;
    std::vector<glm::vec3> m_points;
public:
    Spline(std::string const& name, std::vector<glm::vec3> points);

    std::string const& name() { return m_name; }
    glm::vec3 const& point(int i) { return m_points[i]; }
    size_t const point_count() const { return m_points.size(); }
};

}
