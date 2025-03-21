#include "vertex.h"

namespace Twoc::Nu 
{

ITEM_DEFINE(VertexType0, VERTEX_T0_LIST)

bool VertexType0::validate()
{
    return true;
}

Vertex::Vertex(VertexType0 raw) :
    m_position(raw.position(0), raw.position(1), raw.position(2)),
    m_uv(raw.uv(0), raw.uv(1)),
    m_normal(raw.normal(0), raw.normal(1), raw.normal(2)),
    m_color({raw.color(0), raw.color(1), raw.color(2), raw.color(3)})
{

}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, Color color, glm::vec2 uv) :
    m_position(position),
    m_normal(normal),
    m_color(color),
    m_uv(uv)
{

}

}
