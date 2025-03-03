#pragma once
#include <cstdint>
#include "glm/vec3.hpp"

namespace Twoc::Crate
{

// Source: https://github.com/NoahStolk/twoc-tools/blob/main/src/TwocTools.Core/DataTypes/Crt/CrateType.cs
enum class CrateType
{
    None = -1,
    Empty = 0,
    Default = 1,
    Life = 2,
    AkuAku = 3,
    Arrow = 4,
    QuestionMark = 5,
    Bounce = 6,
    Checkpoint = 7,
    Slot = 8,
    Tnt = 9,
    TimeTrialOne = 10,
    TimeTrialTwo = 11,
    TimeTrialThree = 12,
    IronArrow = 13,
    Exclamation = 14,
    Iron = 15,
    Nitro = 16,
    NitroSwitch = 17,
    Proximity = 18,
    Locked = 19,
    Invincibility = 20,
};

class Crate {
protected:
    glm::vec3 m_position;
    float m_unk1;
    uint16_t m_local_x, m_local_y, m_local_z;
    CrateType m_types[4];
    int16_t m_unk2, m_unk3, m_unk4, m_unk5, m_unk6, m_unk7;
    uint16_t m_exclamation_crate_index;

public:
    Crate(glm::vec3 position, uint16_t local_x, uint16_t local_y, uint16_t local_z, CrateType types[4], uint16_t exclamation_crate_index, float unknown_float, int16_t unknown_shorts[7]);
    ~Crate() = default;

    glm::vec3 position() const { return m_position; }
    float unk1() const { return m_unk1; }
    uint16_t local_x() const { return m_local_x; }
    uint16_t local_y() const { return m_local_y; }
    uint16_t local_z() const { return m_local_z; }
    CrateType type(int i) const { return m_types[i]; }
    uint16_t unk2() const { return m_unk2; }
    uint16_t unk3() const { return m_unk3; }
    uint16_t unk4() const { return m_unk4; }
    uint16_t unk5() const { return m_unk5; }
    uint16_t unk6() const { return m_unk6; }
    uint16_t unk7() const { return m_unk7; }
    uint16_t exclamation_crate_index() const { return m_exclamation_crate_index; }
};
}
