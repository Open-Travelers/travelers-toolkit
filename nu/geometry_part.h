#ifndef GEOMETRY_PART_H
#define GEOMETRY_PART_H
#include "../binary_stream.h"
#include "vertex.h"
#include "geometry_primitive.h"
namespace Nu {

    class GeometryPart {
    public:
        GeometryPart();
        bool read(BinaryStream &stream);

        std::vector<Nu::Vertex> const& get_vertices() { return m_vertices; }
        std::vector<Nu::GeometryPrimitive> const& get_primitives() { return m_primitives; }

        const Nu::Vertex *get_vertices_raw() { return m_vertices.data(); }
        int get_vertex_count() { return m_vertices.size(); }

        const Nu::GeometryPrimitive *get_primitives_raw() { return m_primitives.data(); }
        int get_primitive_count() { return m_primitives.size(); }

        int material_index() { return m_material_index; }
    private:
        bool read_vertices(BinaryStream &stream);
        bool read_skins(BinaryStream &stream);
        bool read_primitives(BinaryStream &stream);
        bool read_control(BinaryStream &stream);
        bool read_blendshapes(BinaryStream &stream);

    protected:
        std::vector<Nu::Vertex> m_vertices;
        std::vector<Nu::GeometryPrimitive> m_primitives;
        int m_material_index { -1 };
    };
}
#endif