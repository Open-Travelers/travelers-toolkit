#include "program_header.h"

namespace Twoc::Elf
{

ITEM_DEFINE(ProgramHeader32, ELF_PROGRAM_HEADER32_ITEM)
ITEM_DEFINE(ProgramHeader64, ELF_PROGRAM_HEADER64_ITEM)

bool ProgramHeader32::validate()
{
    return true;
}

bool ProgramHeader64::validate()
{
    return true;
}

}
