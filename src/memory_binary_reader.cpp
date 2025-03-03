#include <cstring>
#include "memory_binary_reader.h"

MemoryBinaryReader::MemoryBinaryReader(Twoc::ReaderEndianness endianness) : Twoc::BinaryReader(endianness) { }

void MemoryBinaryReader::check_eof()
{
    if (m_cursor >= m_buffer_size)
    {
        m_status = Twoc::ReaderStatus::Eof;
        m_cursor = m_buffer_size;
    }
}

bool MemoryBinaryReader::read_buffer(std::uint8_t *buffer, size_t size)
{
    size_t actual_size = std::min(size, m_buffer_size - m_cursor);
    std::memcpy((void*) buffer, (const void*) m_buffer, actual_size);
    check_eof();
    return size == actual_size;
}

bool MemoryBinaryReader::open(std::string const& path)
{
    m_status = Twoc::ReaderStatus::Error;
    return false;
}

bool MemoryBinaryReader::open(std::uint8_t *buffer, size_t size)
{
    m_buffer = buffer;
    m_buffer_size = size;
    m_cursor = 0;
    m_status = Twoc::ReaderStatus::Ok;
    return true;
}

void MemoryBinaryReader::close()
{
    m_buffer = nullptr;
    m_buffer_size = -1;
    m_status = Twoc::ReaderStatus::Eof;
}

std::size_t MemoryBinaryReader::tell()
{
    return m_cursor;
}

std::size_t MemoryBinaryReader::seek(Twoc::ReaderBase base, std::size_t offset)
{
    ssize_t position = offset;
    switch (base)
    {
    case Twoc::ReaderBase::Start:
        break;
    case Twoc::ReaderBase::Current:
        position += m_cursor;
        break;
    case Twoc::ReaderBase::End:
        position = m_buffer_size - position;
        break;
    }

    if (position < 0)
        position = 0;
    else if (position >= m_buffer_size)
        position = m_buffer_size;

    m_cursor = position;
    check_eof();
    return tell();
}
