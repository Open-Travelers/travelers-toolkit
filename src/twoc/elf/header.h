#ifndef HEADER_H
#define HEADER_H

#include "../reader_item.h"

#define ELFMAG0 (0x7f)
#define ELFMAG1 ('E')
#define ELFMAG2 ('L')
#define ELFMAG3 ('F')

enum {
    EI_MAG0,
    EI_MAG1,
    EI_MAG2,
    EI_MAG3,
    EI_CLASS,
    EI_DATA,
    EI_VERSION,
    EI_OSABI,
    EI_ABIVERSION,
    EI_PAD,
    EI_NIDENT = 16
};

enum {
    ELFCLASSNONE,
    ELFCLASS32,
    ELFCLASS64
};

enum {
    ELFDATANONE,
    ELFDATA2LSB,
    ELFDATA2MSB
};

enum {
    EV_NONE,
    EV_CURRENT
};

enum {
    ELFOSABI_NONE,
    ELFOSABI_SYSV,
    ELFOSABI_HPUX,
    ELFOSABI_NETBSD,
    ELFOSABI_LINUX,
    ELFOSABI_SOLARIS,
    ELFOSABI_IRIX,
    ELFOSABI_FREEBSD,
    ELFOSABI_TRU64,
    ELFOSABI_ARM,
    ELFOSABI_STANDALONE
};

enum {
    ET_NONE,
    ET_REL,
    ET_EXEC,
    ET_DYN,
    ET_CORE
};

enum {
    EM_NONE,
    EM_M32,
    EM_SPARC,
    EM_386,
    EM_68K,
    EM_88K,
    EM_860,
    EM_MIPS,
    EM_PARISC,
    EM_SPARC32PLUS,
    EM_PPC,
    EM_PPC64,
    EM_S390,
    EM_ARM,
    EM_SH,
    EM_SPARCV9,
    EM_IA_64,
    EM_X86_64,
    EM_VAX
};

#define ELF_HEADER_ITEM(i,a) \
    a(char, e_ident, 16) \
    i(uint16_t, e_type) \
    i(uint16_t, e_machine) \
    i(uint32_t, e_version) \
    i(uint32_t, e_entry) \
    i(uint32_t, e_phoff) \
    i(uint32_t, e_shoff) \
    i(uint32_t, e_flags) \
    i(uint16_t, e_ehsize) \
    i(uint16_t, e_phentsize) \
    i(uint16_t, e_phnum) \
    i(uint16_t, e_shentsize) \
    i(uint16_t, e_shnum) \
    i(uint16_t, e_shstrndx)

namespace Twoc::Elf
{
ITEM_DECLARE(ElfHeader, ELF_HEADER_ITEM)
}
#endif // HEADER_H
