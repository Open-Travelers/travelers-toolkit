#pragma once
#include "twoc/binary_reader.h"

class MemoryBinaryReader : public Twoc::BinaryReader
{
protected:
    Twoc::ReaderEndianness m_system_endianness { Twoc::ReaderEndianness::Little };

    std::uint8_t *m_buffer { nullptr };
    std::size_t m_buffer_size { 0 };
    std::size_t m_cursor { 0 };

    void check_eof();
    bool open(std::string const& path) override;
    bool read_buffer(std::uint8_t *buffer, size_t size) override;
public:
    MemoryBinaryReader(Twoc::ReaderEndianness endianness);
    ~MemoryBinaryReader() {}

    bool open(std::uint8_t *buffer, size_t size);
    void close() override;

    std::size_t tell() override;
    std::size_t seek(Twoc::ReaderBase base, std::size_t offset) override;
};
