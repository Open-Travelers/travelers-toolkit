
#ifndef MATERIAL_H
#define MATERIAL_H
#include "../binary_stream.h"
namespace Nu {

    class Material {
    public:
        bool read(BinaryStream& stream);
    protected:
        int m_texture;
        float m_diffuse_r;
        float m_diffuse_g;
        float m_diffuse_b;
    };

}
#endif
