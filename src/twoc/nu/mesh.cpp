#include "mesh.h"

namespace Twoc::Nu {

bool Mesh::read_vertices(Twoc::BinaryReader &reader)
{
    std::vector<Nu::Vertex> vertices;
    uint32_t count = reader.u32();
    for (uint32_t i = 0; i < count; i++)
    {
        VertexType0 raw;
        if (!raw.read(reader))
            return false;

        vertices.push_back(Vertex(raw));
    }

    m_vertices = vertices;
    return true;
}

bool Mesh::read_control(Twoc::BinaryReader &reader)
{
    // huh?
    uint32_t value = reader.u32();
    if (value != 0)
        reader.seek(ReaderBase::Current, value * 0xC);
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Mesh::read_primitives(Twoc::BinaryReader &reader)
{
    std::vector<Primitive> primitives;
    uint32_t count = reader.u32();
    for (uint32_t i = 0; i < count; i++)
    {
        Nu::Primitive prim;
        if (!prim.read(reader))
            return false;
        primitives.push_back(prim);;
    }

    m_primitives = primitives;
    return reader.status() != Twoc::ReaderStatus::Error;
}
bool Mesh::read_skin(Twoc::BinaryReader &reader)
{
    uint32_t count = reader.u32();
    if (count == 0)
        return true;

    if (reader.u8() != 0)
    {
        reader.seek(ReaderBase::Current, m_vertices.size() << 4);
    } else
    {
        for (uint32_t i = 0; i < count; i++)
        {
            uint32_t vertex_offset = reader.u32();
            uint32_t vertex_count = reader.u32();
            uint32_t matrix_count = reader.u32();
            reader.seek(ReaderBase::Current, 4 * matrix_count);
            reader.seek(ReaderBase::Current, (4 * vertex_count) * matrix_count);
        }
    }
    return true;
}

bool Mesh::read_blendshapes(Twoc::BinaryReader &reader)
{
    uint32_t count = reader.u32();
    if (count != 0)
    {
        reader.seek(ReaderBase::Current, count << 2);

        uint32_t some_count = reader.u32();
        reader.seek(ReaderBase::Current, some_count);

        reader.seek(ReaderBase::Current, count);
    }
    return true;
}

bool Mesh::read(Twoc::BinaryReader &reader, size_t fullsize)
{
    m_material = reader.u32();

    // vertex
    if (!read_vertices(reader))
        return false;

    // control
    if (!read_control(reader))
        return false;

    // primitives
    if (!read_primitives(reader))
        return false;

    // skin
    if (!read_skin(reader))
        return false;

    // blendshapes
    if (!read_blendshapes(reader))
        return false;

    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Mesh::validate()
{
    return true;
}

}
