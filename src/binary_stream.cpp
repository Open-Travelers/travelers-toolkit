#include "binary_stream.h"
#include <iostream>
#include <fstream>

BinaryStream::BinaryStream(bool swap_endianness) : m_should_swap_endianness(swap_endianness)
{

}
BinaryStream::~BinaryStream()
{

}

bool BinaryStream::open(const std::string &path) {
    std::ifstream stream(path, std::fstream::binary | std::fstream::ate);
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size + 1);
    if (stream.read((char*) buffer.data(), size))
    {
        m_size = size;
        m_is_open = true;
        m_data = buffer;
        return true;
    }
    return false;
}

std::optional<uint8_t> BinaryStream::read_byte()
{
    if (!m_is_open)
        return std::nullopt;
    if (m_position + 1 >= m_size)
        return std::nullopt;
    return m_data[m_position++];
}
std::optional<uint16_t> BinaryStream::read_half()
{
    if (!m_is_open)
        return std::nullopt;
    if (m_position + 2 >= m_size)
        return std::nullopt;
    uint8_t b1 = m_data[m_position++];
    uint8_t b2 = m_data[m_position++];

    // assumes little endian host
    return (b2 << 8) | b1;
}
std::optional<uint32_t> BinaryStream::read_word()
{
    if (!m_is_open)
        return std::nullopt;
    if (m_position + 4 >= m_size)
        return std::nullopt;
    uint8_t b1 = m_data[m_position++];
    uint8_t b2 = m_data[m_position++];
    uint8_t b3 = m_data[m_position++];
    uint8_t b4 = m_data[m_position++];

    // assumes little endian host
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}
std::optional<float> BinaryStream::read_float()
{
    if (!m_is_open)
        return std::nullopt;
    if (m_position + 4 >= m_size)
        return std::nullopt;
    uint8_t array[4];
    array[0] = m_data[m_position++];
    array[1] = m_data[m_position++];
    array[2] = m_data[m_position++];
    array[3] = m_data[m_position++];
    // D:
    return *(float*) array;
}
std::optional<std::string> BinaryStream::read_zero_terminated_string()
{
    if (!m_is_open)
        return std::nullopt;
    std::vector<uint8_t> bytevec;

    std::optional<uint8_t> c;
    do {
        c = read_byte();
        if (!c.has_value())
            break;
        bytevec.push_back(c.value());
    } while (c.value() != 0);

    if (bytevec.size() == 0)
        return "";
    return std::string(bytevec.begin(), bytevec.end());
}
std::optional<std::string> BinaryStream::read_constant_length_string(size_t length)
{
    if (!m_is_open)
        return std::nullopt;
    if (length >= m_size)
        return std::nullopt;

    std::vector<uint8_t> bytevec;

    std::optional<uint8_t> c;
    do {
        c = read_byte();
        if (!c.has_value())
            break;
        bytevec.push_back(c.value());
    } while ((--length) && (c.value() != 0));

    if (bytevec.size() == 0)
        return "";
    return std::string(bytevec.begin(), bytevec.end());
}
void BinaryStream::seek(int whence, long int offset)
{

    if (whence == SEEK_SET)
        m_position = offset;
    else if (whence == SEEK_CUR)
        m_position += offset;
    else if (whence == SEEK_END)
        m_position = m_size - offset - 1;

    if (m_position < 0)
        m_position = 0;
    if (m_position >= m_size)
        m_position = m_size;
}

#include <cstring>

bool BinaryStream::read(size_t size, uint8_t *output_buffer)
{
    size_t end_position = m_position + size;
    if (size > m_size || end_position >= m_size || is_eof())
        return false;

    std::memcpy(output_buffer, m_data.data() + m_position, size);
    m_position += size;
    return true;
}
bool BinaryStream::is_eof()
{
    return m_position >= m_size;
}
size_t BinaryStream::tell()
{
    return m_position;
}