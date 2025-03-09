#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>

#include "../reader_item.h"

namespace Twoc::Nu 
{

enum InstanceFlags : uint32_t {
    Visible = 1 << 0,
    Onscreen = 1 << 1,
    VisiTest = 1 << 2,
    Animated = 1 << 3
};

#define INSTANCE_ITEM(i, a) \
    a(float, transform_matrix, 16) \
    i(uint32_t, object_index) \
    i(uint32_t, flags) \
    i(uint32_t, p_instanim) \
    i(uint16_t, room_group) \
    i(uint8_t, special_flag) \
    i(uint8_t, padding)

ITEM_DECLARE(InstanceRaw, INSTANCE_ITEM)

class Instance {
protected:
    glm::mat4 m_transform_matrix;
    uint32_t m_object_index;
    InstanceFlags m_flags;
    uint16_t m_room_group;
    uint8_t m_special_flag;

public:
    Instance(InstanceRaw raw);

    glm::mat4 const& transform_matrix() const { return m_transform_matrix; }
    InstanceFlags flags() const { return m_flags; }
    uint32_t object_index() const { return m_object_index; }
    uint8_t special_flag() const { return m_special_flag; }
    uint16_t room_group() const { return m_room_group; }
};

}

