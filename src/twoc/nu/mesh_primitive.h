#pragma once
#include "../reader_item.h"
#include <vector>

namespace Twoc::Nu
{

enum class PrimitiveType {
    TriangleList = 5,
    TriangleStrip = 6
};

class Primitive : public IReaderItem {
protected:
    std::vector<uint16_t> m_indices;
    PrimitiveType m_type;

    bool validate() override;
public:
    Primitive() = default;
    ~Primitive() = default;

    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    size_t size() override { return 0; }
};
}
