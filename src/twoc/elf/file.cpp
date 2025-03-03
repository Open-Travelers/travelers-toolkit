#include <iostream>
#include "file.h"

namespace Twoc::Elf
{

File::~File()
{
    for (IProgramHeader *iheader : m_program_headers)
        delete iheader;
    for (ISectionHeader *iheader : m_section_headers)
        delete iheader;

    for (auto &val : m_symbol_map)
        delete val.second;
}

size_t File::address_to_offset(size_t address)
{
    if (!m_done)
        return 0;

    for (ISectionHeader* iheader : m_section_headers)
    {
        SectionHeader32 *header = (SectionHeader32*) iheader;
        if (address < header->sh_addr() || address >= header->sh_addr() + header->sh_size())
            continue;

        return address - header->sh_addr() + header->sh_offset();
    }
    return 0;
}

MinimumSymbol File::find_symbol(std::string const& name)
{
    if (!m_done)
        return { 0, 0 };

    if (!m_symbol_map.count(name))
        return { 0, 0 };

    Symbol32 *sym = (Symbol32*) m_symbol_map[name];
    return { sym->st_value(), sym->st_size() };

    return { 0, 0 };
}

bool File::read(BinaryReader &reader, size_t realsize)
{
    m_done = false;
    if (!m_header.read(reader))
        return false;
    if (m_header.e_ident(EI_CLASS) != ELFCLASS32)
        return false;

    if (m_header.e_phnum() != 0)
    {
        reader.seek(ReaderBase::Start, m_header.e_phoff());
        m_program_headers.resize(m_header.e_phnum());
        for (int i = 0; i < m_header.e_phnum(); i++)
        {
            ProgramHeader32 *hdr = ProgramHeader32::from_reader(reader, m_header.e_phentsize());
            if (!hdr)
                return false;
            m_program_headers[i] = hdr;
        }
    }

    if (m_header.e_shnum() != 0)
    {
        ISectionHeader* isymtab_section = nullptr;
        ISectionHeader* istrtab_section = nullptr;

        reader.seek(ReaderBase::Start, m_header.e_shoff());
        m_section_headers.resize(m_header.e_shnum());
        for (int i = 0; i < m_header.e_shnum(); i++)
        {
            SectionHeader32 *hdr = SectionHeader32::from_reader(reader, m_header.e_shentsize());
            if (!hdr)
                return false;
            if (hdr->sh_type() == SHT_SYMTAB)
                isymtab_section = hdr;
            else if (hdr->sh_type() == SHT_STRTAB)
                istrtab_section = hdr;
            m_section_headers[i] = hdr;
        }

        if (isymtab_section != nullptr && istrtab_section != nullptr)
        {
            SectionHeader32 *symtab = (SectionHeader32*) isymtab_section;
            SectionHeader32 *strtab = (SectionHeader32*) istrtab_section;

            size_t symbol_count = symtab->sh_size() / symtab->sh_entsize();
            reader.seek(ReaderBase::Start, symtab->sh_offset());
            for (int i = 0; i < symbol_count; i++)
            {
                Symbol32 *symbol = Symbol32::from_reader(reader, symtab->sh_entsize());
                if (symbol->st_name() == 0)
                    continue;
                size_t old = reader.tell();
                reader.seek(ReaderBase::Start, strtab->sh_offset() + symbol->st_name());

                std::string name = reader.zero_terminated_string();
                m_symbol_map[name] = symbol;

                reader.seek(ReaderBase::Start, old);
            }
        }
    }

    m_done = true;
    return true;
}

bool File::validate()
{
    return true;
}

}
