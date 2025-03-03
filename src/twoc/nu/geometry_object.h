#pragma once
#include <glm/vec3.hpp>
#include "../reader_item.h"
#include "mesh.h"

namespace Twoc::Nu {

class GeometryObject : public IReaderItem {
protected:
    glm::vec3 m_origin;
    std::vector<Mesh> m_meshes;
    bool validate() override;
public:
    GeometryObject() = default;
    ~GeometryObject() = default;

    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    size_t size() override { return 0; }

    glm::vec3 const& origin() { return m_origin; }
    std::vector<Mesh> const& meshes() { return m_meshes; }
};

}
