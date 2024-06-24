
#include "vertex.h"
#include <iostream>
namespace Nu {

bool Vertex::read(BinaryStream &stream)
{
    if (!m_position.read(stream))
        return false;
    //m_position.set_z(m_position.z() * -1);
    
    if (!m_normal.read(stream))
        return false;
    
    auto rgba = stream.read_word();
    if (!rgba.has_value()) 
        return false;
    m_rgba = rgba.value();
    //std::cout << std::to_string(m_rgba) << std::endl;
    if (!m_uv.read(stream))
        return false;
    return true;
}

Vertex::Vertex() : m_position({0, 0, 0}), m_uv({0, 0}), m_rgba(0), m_normal({0, 0, 0}) {}
Vertex::Vertex(Vec3 position, Vec3 normal, Vec2 uv, uint32_t color) : m_position(position), m_normal(normal), m_uv(uv), m_rgba(color) {}
Vec3 Vertex::position() { return m_position; }
Vec3 Vertex::normal() { return m_normal; }
Vec2 Vertex::uv() { return m_uv; }
uint32_t Vertex::rgba() { return m_rgba; }

void Vertex::set_position(Vec3 position) { m_position = position; }
void Vertex::set_normal(Vec3 normal) { m_normal = normal; }
void Vertex::set_uv(Vec2 uv) { m_uv = uv; }
void Vertex::set_rgba(uint32_t rgba) { m_rgba = rgba; }

}