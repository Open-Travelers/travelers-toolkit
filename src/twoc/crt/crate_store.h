#pragma once
#include "crate.h"
#include "crate_group.h"
#include "../binary_reader.h"

namespace Twoc::Crate
{

class CrateStore {
protected:
public:
    explicit CrateStore() = default;
    ~CrateStore();

    static CrateStore *from_reader(BinaryReader &reader);
    bool read(BinaryReader &reader);
};

}
