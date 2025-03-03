#include "section_header.h"

namespace Twoc::Elf
{

ITEM_DEFINE(SectionHeader32, ELF_SECTION_HEADER32_ITEM)
ITEM_DEFINE(SectionHeader64, ELF_SECTION_HEADER64_ITEM)

bool SectionHeader32::validate() { return true; }
bool SectionHeader64::validate() { return true; }

}
