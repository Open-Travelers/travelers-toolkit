#ifndef GEOMETRY_PRIMITIVE_H
#define GEOMETRY_PRIMITIVE_H
#include "../binary_stream.h"
namespace Nu
{


struct GeometryPrimitiveFace
{
    unsigned int count;
    int data_offset;
};

class GeometryPrimitive
{
public:
    enum class Type {
        tPrimInvalid = 0,
        tPrimTriList = 5,
        tPrimTriStrip = 6,
        tPrimTriBezier = 8
    };

public:
    GeometryPrimitive();
    bool read(BinaryStream &stream);

    Type get_type() { return m_type; }
    uint16_t *get_data_raw() { return m_indices.data(); }
    int get_data_count() { return m_indices.size(); }

    std::vector<GeometryPrimitiveFace> const& get_faces() { return m_faces; }
    GeometryPrimitiveFace *get_faces_raw() { return m_faces.data(); }
    int get_faces_count() { return m_faces.size(); }
protected:
    Type m_type { Type::tPrimInvalid };
    std::vector<uint16_t> m_indices;
    std::vector<GeometryPrimitiveFace> m_faces;
};

}
#endif