#include "mesh_primitive.h"

namespace Twoc::Nu {

bool Primitive::read(Twoc::BinaryReader &reader, size_t fullsize)
{
    uint32_t type = reader.u32();
    uint32_t index_count = reader.u32();

    std::vector<uint16_t> indices;
    for (uint16_t j = 0; j < index_count; j++)
        indices.push_back(reader.u16());

    m_indices = indices;
    m_type = (PrimitiveType) type;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Primitive::validate()
{
    return true;
}
}
