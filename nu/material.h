
#ifndef MATERIAL_H
#define MATERIAL_H
#include "../binary_stream.h"
#include "../math/vec4.h"
namespace Nu {
    // Source: https://github.com/Open-Travelers/OpenCrashWOC/blob/main/code/src/nu3dx/nu3dxtypes.h
    struct Attributes
    {
        unsigned int alpha : 2; // Offset: 0x0, Bit Offset: 0, Bit Size: 2
        unsigned int filter : 2; // Offset: 0x0, Bit Offset: 2, Bit Size: 2
        unsigned int fx : 2; // Offset: 0x0, Bit Offset: 4, Bit Size: 2
        unsigned int utc : 2; // Offset: 0x0, Bit Offset: 6, Bit Size: 2
        unsigned int vtc : 2; // Offset: 0x0, Bit Offset: 8, Bit Size: 2
        unsigned int cull : 2; // Offset: 0x0, Bit Offset: 10, Bit Size: 2
        unsigned int zmode : 2; // Offset: 0x0, Bit Offset: 12, Bit Size: 2
        unsigned int lighting : 2; // Offset: 0x0, Bit Offset: 14, Bit Size: 2
        unsigned int colour : 1; // Offset: 0x0, Bit Offset: 16, Bit Size: 1
        unsigned int fill : 1; // Offset: 0x0, Bit Offset: 17, Bit Size: 1
        unsigned int atst : 3; // Offset: 0x0, Bit Offset: 18, Bit Size: 3
        unsigned int aref : 8; // Offset: 0x0, Bit Offset: 21, Bit Size: 8
        unsigned int afail : 2; // Offset: 0x0, Bit Offset: 29, Bit Size: 2
        unsigned int uvmode : 1; // Offset: 0x0, Bit Offset: 31, Bit Size: 1
    };
    class Material {
    public:
        Material();
        bool read(BinaryStream& stream);
        Attributes attributes() { return m_attributes; }
        Vec3 &diffuse() { return m_diffuse; }
        Vec3 &ambient() { return m_ambient; }
        float power() { return m_power; }
        float alpha() { return m_alpha; }
        int texture() { return m_texture; }
    protected:
        float m_power { 1.0f };
        float m_alpha { 1.0f };
        int m_texture { -1 };
        Vec3 m_ambient { Vec3(1.f, 1.f, 1.f) };
        Vec3 m_diffuse { Vec3(1.f, 1.f, 1.f) };
        Attributes m_attributes { 0 };
    };

}
#endif
