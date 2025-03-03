#include "header.h"
namespace Twoc::Elf
{

ITEM_DEFINE(ElfHeader,ELF_HEADER_ITEM)

bool ElfHeader::validate()
{
    if (e_ident(EI_MAG0) != ELFMAG0 || e_ident(EI_MAG1) != ELFMAG1 || e_ident(EI_MAG2) != ELFMAG2 || e_ident(EI_MAG3) != ELFMAG3)
        return false;
    if (e_ident(EI_CLASS) == ELFCLASSNONE)
        return false;
    if (e_ident(EI_DATA) == ELFDATANONE)
        return false;
    if (e_ident(EI_VERSION) != EV_CURRENT)
        return false;
    return true;
}
}
