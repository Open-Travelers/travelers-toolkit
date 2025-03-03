#ifndef PROGRAM_HEADER_H
#define PROGRAM_HEADER_H

#include "../reader_item.h"

#define ELF_PROGRAM_HEADER32_ITEM(i, a) \
    i(uint32_t, p_type) \
    i(uint32_t, p_offset) \
    i(uint32_t, p_vaddr) \
    i(uint32_t, p_paddr) \
    i(uint32_t, p_filesz) \
    i(uint32_t, p_memsz) \
    i(uint32_t, p_flags) \
    i(uint32_t, p_align)

#define ELF_PROGRAM_HEADER64_ITEM(i, a) \
    i(uint32_t, p_type) \
    i(uint32_t, p_flags) \
    i(uint64_t, p_offset) \
    i(uint64_t, p_vaddr) \
    i(uint64_t, p_paddr) \
    i(uint64_t, p_filesz) \
    i(uint64_t, p_memsz) \
    i(uint64_t, p_align)

namespace Twoc::Elf
{

enum {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_LOPROC,
    PT_HIPROC,
    PT_GNU_STACK
};

class IProgramHeader : public IReaderItem { };
ITEM_DECLARE_EX(ProgramHeader32, IProgramHeader, ELF_PROGRAM_HEADER32_ITEM)
ITEM_DECLARE_EX(ProgramHeader64, IProgramHeader, ELF_PROGRAM_HEADER64_ITEM)

}
#endif // PROGRAM_HEADER_H
