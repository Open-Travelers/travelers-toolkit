#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace Twoc::Nu {

class Spline {
protected:
    std::string m_name { "" };
    size_t m_name_index { 0 };
    std::vector<glm::vec3> m_points;

    friend class Scene;
    void update_name(std::string name) { m_name = name; }
public:
    Spline(size_t name_index, std::vector<glm::vec3> points);
    
    std::string const& name() const { return m_name; }
    std::vector<glm::vec3> const& points() const { return m_points; }
    glm::vec3 const& point(int i) const { return m_points[i]; }
    size_t const point_count() const { return m_points.size(); }
};

}
