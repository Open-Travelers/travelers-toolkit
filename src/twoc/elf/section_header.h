#ifndef SECTION_HEADER_H
#define SECTION_HEADER_H

#include "../reader_item.h"

#define ELF_SECTION_HEADER32_ITEM(i, a) \
    i(uint32_t, sh_name) \
    i(uint32_t, sh_type) \
    i(uint32_t, sh_flags) \
    i(uint32_t, sh_addr) \
    i(uint32_t, sh_offset) \
    i(uint32_t, sh_size) \
    i(uint32_t, sh_link) \
    i(uint32_t, sh_info) \
    i(uint32_t, sh_addralign) \
    i(uint32_t, sh_entsize)

#define ELF_SECTION_HEADER64_ITEM(i, a) \
    i(uint32_t, sh_name) \
    i(uint32_t, sh_type) \
    i(uint64_t, sh_flags) \
    i(uint64_t, sh_addr) \
    i(uint64_t, sh_offset) \
    i(uint64_t, sh_size) \
    i(uint32_t, sh_link) \
    i(uint32_t, sh_info) \
    i(uint64_t, sh_addralign) \
    i(uint64_t, sh_entsize)

namespace Twoc::Elf
{

enum {
    SHT_NULL,
    SHT_PROGBITS,
    SHT_SYMTAB,
    SHT_STRTAB,
    SHT_RELA,
    SHT_HASH,
    SHT_DYNAMIC,
    SHT_NOTE,
    SHT_NOBITS,
    SHT_REL,
    SHT_SHLIB,
    SHT_DYNSYM,
    SHT_LOPROC,
    SHT_HIPROC,
    SHT_LOUSER,
    SHT_HIUSER
};

enum {
    SHF_WRITE = 1 << 0,
    SHF_ALLOC = 1 << 1,
    SHF_EXECINSTR = 1 << 2,
    SHF_MASKPROC = 1 << 3
};


class ISectionHeader : public IReaderItem { };
ITEM_DECLARE_EX(SectionHeader32, ISectionHeader, ELF_SECTION_HEADER32_ITEM)
ITEM_DECLARE_EX(SectionHeader64, ISectionHeader, ELF_SECTION_HEADER64_ITEM)

}
#endif // SECTION_HEADER_H
