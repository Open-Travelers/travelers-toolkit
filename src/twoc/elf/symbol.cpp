#include "symbol.h"

namespace Twoc::Elf
{

ITEM_DEFINE(Symbol32, ELF_SYMBOL32_ITEM)
ITEM_DEFINE(Symbol64, ELF_SYMBOL64_ITEM)

bool Symbol32::validate() { return true; }
bool Symbol64::validate() { return true; }

}
