#pragma once
#include <fstream>
#include "twoc/binary_reader.h"

class FileBinaryReader : public Twoc::BinaryReader
{
protected:
    std::ifstream m_stream;

    bool read_buffer(uint8_t *buffer, size_t size) override;
public:
    FileBinaryReader(Twoc::ReaderEndianness endianness);
    ~FileBinaryReader() override = default;

    bool open(std::string const& path) override;
    void close() override;

    std::size_t tell() override;
    std::size_t seek(Twoc::ReaderBase base, std::size_t offset) override;
};
