#pragma once

#include <cstdint>
#include "../reader_item.h"
#include "vertex.h"
#include "mesh_primitive.h"

namespace Twoc::Nu {

class Mesh : public IReaderItem {
protected:
    uint32_t m_material;
    std::vector<Nu::Vertex> m_vertices;
    std::vector<Nu::Primitive> m_primitives;

    bool validate() override;
    bool read_vertices(Twoc::BinaryReader &reader);
    bool read_control(Twoc::BinaryReader &reader);
    bool read_primitives(Twoc::BinaryReader &reader);
    bool read_skin(Twoc::BinaryReader &reader);
    bool read_blendshapes(Twoc::BinaryReader &reader);
public:
    ~Mesh() override = default;
    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    size_t size() override { return 0; }

    uint32_t material() const { return m_material; }
    std::vector<Nu::Vertex> const& vertices() { return m_vertices; }
    std::vector<Nu::Primitive> const& primitives() { return m_primitives; }
};
}
