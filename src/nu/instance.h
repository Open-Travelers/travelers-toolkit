#ifndef INSTANCE_H
#define INSTANCE_H
#include "../binary_stream.h"
#include "../math/mat4x4.h"

namespace Nu
{

    class Instance {
    public:
        Instance();
        bool read(BinaryStream &stream);
        
        int get_geometry_object();
        Mat4x4 get_transform_matrix();
        bool is_unused() { return m_is_unused; }
        void set_unused() { m_is_unused = true; }
    protected:
        int m_geometry_object;
        Mat4x4 m_transform_matrix;
        bool m_is_unused { false };
    };

}

#endif