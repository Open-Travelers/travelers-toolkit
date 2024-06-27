#include "geometry_primitive.h"

namespace Nu {
    GeometryPrimitive::GeometryPrimitive() { }
    bool GeometryPrimitive::read(BinaryStream &stream)
    {
        auto type = stream.read_word();
        if (!type.has_value()) return false;

        auto count = stream.read_word();
        if (!count.has_value()) return false;
        
        m_indices.resize(count.value());
        stream.read(count.value() * 2, (uint8_t*) m_indices.data());

        if ((GeometryPrimitive::Type) type.value() == GeometryPrimitive::Type::tPrimTriStrip)
        {
            int i = 0;
            while (i < count.value())
            {
                uint32_t count = m_indices[i++];
                if (count < 3) continue;

                m_faces.push_back(GeometryPrimitiveFace(count, i));
                i += count;
            }

        } else {
            m_faces.push_back(GeometryPrimitiveFace(count.value(), 0 ));
        }
        m_type = (GeometryPrimitive::Type) type.value();
        return true;
    }
}