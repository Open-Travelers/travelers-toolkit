#include "file_binary_reader.h"

FileBinaryReader::FileBinaryReader(Twoc::ReaderEndianness endianness) : Twoc::BinaryReader(endianness) { }

bool FileBinaryReader::open(std::string const& path)
{
    m_status = Twoc::ReaderStatus::Ok;
    m_stream.open(path, std::ios::binary | std::ios::in);
    return m_stream.is_open();
}

void FileBinaryReader::close()
{
    m_stream.close();
}

bool FileBinaryReader::read_buffer(uint8_t *buffer, size_t size)
{
    m_stream.read(reinterpret_cast<char*>(buffer), size);
    if (m_stream.tellg() == EOF)
        m_status = Twoc::ReaderStatus::Eof;
    return true;
}

std::size_t FileBinaryReader::tell()
{
    return m_stream.tellg();
}

std::size_t FileBinaryReader::seek(Twoc::ReaderBase base, std::size_t offset)
{
    std::ios_base::seekdir dir;
    switch (base)
    {
    case Twoc::ReaderBase::Start:
        dir = m_stream.beg;
        break;
    case Twoc::ReaderBase::Current:
        dir = m_stream.cur;
        break;
    case Twoc::ReaderBase::End:
        dir = m_stream.end;
        break;
    }

    m_stream.clear();
    m_stream.seekg(offset, dir);
    if (m_stream.tellg() == EOF)
        m_status = Twoc::ReaderStatus::Eof;

    return tell();
}
