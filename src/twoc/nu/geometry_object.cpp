#include "geometry_object.h"

namespace Twoc::Nu {

bool GeometryObject::read(BinaryReader &reader, size_t fullsize)
{
    float origin_x = reader.f32();
    float origin_y = reader.f32();
    float origin_z = reader.f32();
    m_origin = { origin_x, origin_y, origin_z };

    uint32_t mesh_geometry_count = reader.u32();
    for (int z = 0; z < mesh_geometry_count; z++)
    {
        Mesh mesh;
        if (!mesh.read(reader))
        {
            std::cerr << "Failed to read mesh " << z << std::endl;
            return false;
        }
        m_meshes.push_back(mesh);
    }
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool GeometryObject::validate()
{
    return true;
}
}
