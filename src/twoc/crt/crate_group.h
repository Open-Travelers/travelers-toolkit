#pragma once
#include <cstdint>
#include <vector>
#include "glm/vec3.hpp"

namespace Twoc::Crate
{

class CrateGroup
{
protected:
    glm::vec3 m_position;
    uint16_t m_tilt;
    uint16_t m_crate_offset;
    std::vector<uint16_t> m_crate_indices;

public:
    CrateGroup(glm::vec3 position, uint16_t tilt, uint16_t crate_offset, std::vector<uint16_t> crate_indices);
    ~CrateGroup() = default;

    glm::vec3 position() const { return m_position; }
    uint16_t tilt() const { return m_tilt; }
    uint16_t crate_offset() const { return m_crate_offset; }
    std::vector<uint16_t> const& crate_indices() { return m_crate_indices; }
    uint16_t crate_index(int i) { return m_crate_indices[i]; }
};

}
