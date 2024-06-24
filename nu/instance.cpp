
#include "instance.h"

namespace Nu {

    Instance::Instance() {}

    bool Instance::read(BinaryStream &stream)
    {
        if (!m_transform_matrix.read(stream))
            return false;
        
        auto model_index = stream.read_word();
        if (!model_index.has_value())
            return false;
        m_geometry_object = model_index.value();
        
        /// TODO: figure this one out
        stream.seek(SEEK_CUR, 12);
        return true;

    }
    int Instance::get_geometry_object() { return m_geometry_object; }
    Mat4x4 Instance::get_transform_matrix() { return m_transform_matrix; }

}