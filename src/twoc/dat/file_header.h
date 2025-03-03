#pragma once

#include <cstdint>
#include "../reader_item.h"

namespace Twoc::Dat {

#define FILE_HEADER_ITEM(i, a) \
    i(uint32_t, cursor) \
    i(uint32_t, memory_buffer_end) \
    i(uint32_t, size) \
    i(uint32_t, p_next) \
    i(uint32_t, memory_buffer_size) \
    i(uint32_t, is_simple_file) \
    i(uint32_t, p_memory_buffer) \
    i(uint32_t, p_tree)

ITEM_DECLARE(FileHeaderRaw, FILE_HEADER_ITEM)

}
