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
public:
    struct Face {
        size_t Count;
        size_t Index;
        Face(size_t count, size_t index) : Count(count), Index(index) {}
    };
protected:
    std::vector<uint16_t> m_indices;
    std::vector<Face> m_faces;
    PrimitiveType m_type;

    bool validate() override;

public:
    Primitive() = default;
    ~Primitive() = default;

    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    size_t size() override { return 0; }

    PrimitiveType type() const { return m_type; }
    std::vector<uint16_t> const& indices() const { return m_indices; }
    std::vector<Face> const& faces() const { return m_faces; }
};
}
