#ifndef SYMBOL_H
#define SYMBOL_H
#include "../reader_item.h"

#define ELF_SYMBOL32_ITEM(i, a) \
    i(uint32_t, st_name) \
    i(uint32_t, st_value) \
    i(uint32_t, st_size) \
    i(uint8_t, st_info) \
    i(uint8_t, st_other) \
    i(uint16_t, st_shndx)

#define ELF_SYMBOL64_ITEM(i, a) \
    i(uint32_t, st_name) \
    i(uint8_t, st_info) \
    i(uint8_t, st_other) \
    i(uint16_t, st_shndx) \
    i(uint64_t, st_value) \
    i(uint64_t, st_size)

namespace Twoc::Elf
{

class ISymbol : public IReaderItem {};
ITEM_DECLARE_EX(Symbol32, ISymbol, ELF_SYMBOL32_ITEM)
ITEM_DECLARE_EX(Symbol64, ISymbol, ELF_SYMBOL64_ITEM)

struct MinimumSymbol
{
    uint64_t Value;
    uint64_t Size;
};

}
#endif // SYMBOL_H
