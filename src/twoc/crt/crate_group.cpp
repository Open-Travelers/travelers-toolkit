#include "crate_group.h"

namespace Twoc::Crate
{

CrateGroup::CrateGroup(glm::vec3 position, uint16_t tilt, uint16_t crate_offset, std::vector<uint16_t> crate_indices)
    : m_position(position), m_tilt(tilt), m_crate_offset(crate_offset), m_crate_indices(crate_indices)
{
}

}
