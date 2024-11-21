#ifndef GEOMETRY_DEFINITION_H
#define GEOMETRY_DEFINITION_H
#include "../binary_stream.h"
#include "geometry_part.h"
namespace Nu {

    class GeometryDefinition {
    public:
        GeometryDefinition();
        bool read(BinaryStream &stream);
        std::vector<GeometryPart> const& get_parts() { return m_parts; } 

        GeometryPart *get_parts_raw() { return m_parts.data(); }
        int get_parts_count() { return m_parts.size(); }
        bool is_unused() { return m_is_unused; }
        void set_unused() { m_is_unused = true; }
    private:
        std::vector<GeometryPart> m_parts;
        bool m_is_unused { false };
    };
}

#endif