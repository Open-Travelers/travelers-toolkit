
#ifndef VERTEX_H
#define VERTEX_H
#include "../binary_stream.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

namespace Nu {

class Vertex
{
protected:
    Vec3 m_position;
    Vec3 m_normal;
    Vec2 m_uv;
    uint32_t m_rgba;
public:
    Vec3 position();
    Vec3 normal();
    Vec2 uv();
    uint32_t rgba();

    void set_position(Vec3 position);
    void set_normal(Vec3 normal);
    void set_uv(Vec2 uv);
    void set_rgba(uint32_t rgba);
public:
    bool read(BinaryStream &stream);
    Vertex();
    Vertex(Vec3 position, Vec3 normal, Vec2 uv, uint32_t color);
};


}

#endif