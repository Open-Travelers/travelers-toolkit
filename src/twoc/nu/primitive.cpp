#include "primitive.h"

namespace Twoc::Nu {

bool Primitive::read(Twoc::BinaryReader &reader, size_t fullsize)
{
    uint32_t type = reader.u32();
    uint32_t index_count = reader.u32();

    std::vector<uint16_t> indices = reader.array<uint16_t>(index_count);
    m_faces.clear();
    if (type == (int) PrimitiveType::TriangleStrip)
    {
        size_t i = 0;
        while (i < index_count)
        {
            uint32_t count = indices[i++];
            if (count < 3) continue;

            m_faces.push_back(Face(count, i));
            i += count;
        }
    } else {
        m_faces.push_back(Face(index_count, 0));
    }

    m_indices = indices;
    m_type = (PrimitiveType) type;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Primitive::validate()
{
    return true;
}
}
