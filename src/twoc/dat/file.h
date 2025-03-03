#pragma once

#include "../reader_item.h"

namespace Twoc::Dat
{

class File : public IReaderItem
{
public:
    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0);
    size_t size() override { return 0; }

protected:
    bool validate() override;

};

}
