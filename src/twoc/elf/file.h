#ifndef FILE_H
#define FILE_H

#include "header.h"
#include "program_header.h"
#include "section_header.h"
#include "symbol.h"
#include <map>

namespace Twoc::Elf
{

class File : IReaderItem
{
public:
    ~File() override;
    bool read(BinaryReader &reader, size_t realsize = 0) override;
    bool validate() override;

    size_t address_to_offset(size_t address);
    MinimumSymbol find_symbol(std::string const& name);
protected:
    size_t size() { return 0; }
    ElfHeader m_header;
    std::vector<ISectionHeader*> m_section_headers;
    std::vector<IProgramHeader*> m_program_headers;
    std::map<std::string, ISymbol*> m_symbol_map;
    bool m_done { false };
};

}

#endif // FILE_H
