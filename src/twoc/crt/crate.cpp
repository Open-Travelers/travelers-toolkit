
#include "crate.h"

namespace Twoc::Crate
{

Crate::Crate(glm::vec3 position, uint16_t local_x, uint16_t local_y, uint16_t local_z, CrateType types[4], uint16_t exclamation_crate_index, float unknown_float, int16_t unknown_shorts[7])
    : m_position(position), m_local_x(local_x), m_local_y(local_y), m_local_z(local_z), m_exclamation_crate_index(exclamation_crate_index), m_unk1(unknown_float),
    m_unk2(unknown_shorts[0]), m_unk3(unknown_shorts[1]), m_unk4(unknown_shorts[2]), m_unk5(unknown_shorts[3]), m_unk6(unknown_shorts[4]), m_unk7(unknown_shorts[5])
{
    m_types[0] = types[0];
    m_types[1] = types[1];
    m_types[2] = types[2];
    m_types[3] = types[3];
}

}
