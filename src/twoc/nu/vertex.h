
#ifndef VERTEX_H
#define VERTEX_H

#include "../reader_item.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Twoc::Nu 
{

#define VERTEX_T0_LIST(i, a) \
    a(float, position, 3) \
    a(float, normal, 3) \
    a(uint8_t, color, 4) \
    a(float, uv, 2)

ITEM_DECLARE(VertexType0, VERTEX_T0_LIST)

struct Color {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

class Vertex {
protected:
    glm::vec3 m_position;
    glm::vec3 m_normal;
    Color m_color;
    glm::vec2 m_uv;

public:
    static constexpr size_t PositionOffset = 0;
    static constexpr size_t NormalOffset = PositionOffset + sizeof(glm::vec3);
    static constexpr size_t ColorOffset = NormalOffset + sizeof(glm::vec3);
    static constexpr size_t UvOffset = ColorOffset + sizeof(Color);

    Vertex(VertexType0 raw);
    Vertex(glm::vec3 position, glm::vec3 normal, Color color, glm::vec2 uv);

    glm::vec3 const& position() { return m_position; }
    glm::vec3 const& normal() { return m_normal; }
    glm::vec2 const& uv() { return m_uv; }
    Color const& color() { return m_color; }
};

}
#endif
