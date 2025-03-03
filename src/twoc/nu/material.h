
#ifndef MATERIAL_H
#define MATERIAL_H

#include "../reader_item.h"

namespace Twoc::Nu 
{
#define MATERIAL_ITEM(i, a) \
    i(uint32_t, p_next) \
    i(uint32_t, flags) \
    a(float, ambient, 3) \
    a(float, diffuse, 3) \
    a(uint32_t, fx_params, 4) \
    i(float, power) \
    i(float, alpha) \
    i(uint32_t, texture_id) \
    i(int16_t, alpha_sort) \
    i(uint8_t, fx_id) \
    i(uint8_t, special_id) \
    i(int16_t, K) \
    i(uint8_t, L) \
    i(uint8_t, animation_mode) \
    i(float, du) \
    i(float, dv) \
    i(float, su) \
    i(float, sv)

ITEM_DECLARE(Material, MATERIAL_ITEM)

}
#endif
