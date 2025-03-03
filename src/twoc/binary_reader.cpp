#include <algorithm>
#include "binary_reader.h"

namespace Twoc
{

static void swap_bytes(std::uint8_t *array, int size)
{
    std::reverse(array + 0, array + size);
}

BinaryReader::BinaryReader(ReaderEndianness endianness) : m_file_endianness(endianness) {
    union {
        uint8_t array[4];
        uint32_t value;
    } u;

    u.value = 0x01020304;
    if (u.array[0] == 0x01)
        m_system_endianness = Twoc::ReaderEndianness::Big;
}

std::string BinaryReader::zero_terminated_string()
{
    std::string s = "";
    std::int8_t v = s8();
    while (v != 0) {
        s += v;
        v = s8();
        if (m_status != Twoc::ReaderStatus::Ok)
            break;
    }
    return s;
}

std::uint8_t BinaryReader::u8()
{
    std::uint8_t val = 0;
    if (!read_buffer(reinterpret_cast<uint8_t*>(&val), 1))
        return 0;
    return val;
}

std::uint16_t BinaryReader::u16()
{
    std::uint16_t val = 0;
    if (!read_buffer(reinterpret_cast<uint8_t*>(&val), 2))
        return 0;

    if (m_system_endianness != m_file_endianness)
        swap_bytes(reinterpret_cast<std::uint8_t*>(&val), 2);

    return val;
}

std::uint32_t BinaryReader::u32()
{
    std::uint32_t val = 0;
    if (!read_buffer(reinterpret_cast<uint8_t*>(&val), 4))
        return 0;

    if (m_system_endianness != m_file_endianness)
        swap_bytes(reinterpret_cast<std::uint8_t*>(&val), 4);

    return val;
}

float BinaryReader::f32()
{
    float val = 0;
    if (!read_buffer(reinterpret_cast<uint8_t*>(&val), 4))
        return 0;

    if (m_system_endianness != m_file_endianness)
        swap_bytes(reinterpret_cast<std::uint8_t*>(&val), 4);

    return val;
}

void* BinaryReader::ptr(ptrdiff_t initial_base, void* target_base)
{
    std::uint32_t offset = u32();
    return (void*) (((uint8_t*) target_base) + offset);

}

}
