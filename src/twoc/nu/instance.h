
#ifndef INSTANCE_H
#define INSTANCE_H

#include <cstdint>
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

ITEM_DECLARE(Instance, INSTANCE_ITEM)

}

#endif
