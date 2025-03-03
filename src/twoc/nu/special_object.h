#pragma once

#include "../reader_item.h"
namespace Twoc::Nu {

#define SPECIAL_OBJECT_ITEM(i, a) \
    a(float, matrix, 16) \
    i(uint32_t, p_instance) \
    i(uint32_t, p_name) \
    i(uint32_t, flags) \
    i(uint32_t, padding)

ITEM_DECLARE(SpecialObjectRaw, SPECIAL_OBJECT_ITEM)

}
