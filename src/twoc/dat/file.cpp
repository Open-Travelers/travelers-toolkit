#include "file.h"
#include "file_header.h"

namespace Twoc::Dat
{
bool File::read(BinaryReader &reader, size_t realsize)
{
    uint32_t filetable_offset = reader.u32();
    uint32_t filetable_size = reader.u32();

    int i = 0;
    while (filetable_size > 0x20)
    {
        reader.seek(ReaderBase::Start, filetable_offset + i * 0x20);
//        std::cout << "Header " << i << std::endl;
        FileHeaderRaw header;
        if (!header.read(reader))
            return false;

        std::cout << "Cursor: " << header.cursor() << std::endl;
        std::cout << "Memory Buffer: " << header.p_memory_buffer() << std::endl;
        std::cout << "Memory Buffer Size: " << header.memory_buffer_size() << std::endl;

        std::cout << "Memory Buffer End: " << header.memory_buffer_end() << std::endl;
        std::cout << "Is Simple File: " << header.is_simple_file() << std::endl;
        std::cout << "Tree: " << header.p_tree() << std::endl;
        std::cout << "Next: " << header.p_next() << std::endl;

        if (header.memory_buffer_end() != 0)
        {
            reader.seek(ReaderBase::Start, filetable_offset + header.memory_buffer_end() + 6817);
            std::string filepath = reader.zero_terminated_string();
            std::cout << "Filepath: " << filepath << std::endl;
        } else {
            std::cout << "Header " << i << " skipped; seemingly empty" << std::endl;
        }

        filetable_size -= 0x20;
        i++;
    }
    return true;
}
bool File::validate()
{
    return true;
}
}
