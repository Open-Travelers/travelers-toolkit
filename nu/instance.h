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
    protected:
        int m_geometry_object;
        Mat4x4 m_transform_matrix;
    };

}

#endif