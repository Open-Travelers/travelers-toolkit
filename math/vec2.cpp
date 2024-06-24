#include "vec2.h"
#include <cmath>
float Vec2::x() const { return m_data[0]; }
float Vec2::y() const { return m_data[1]; }
void Vec2::set_x(float value) { m_data[0] = value; }
void Vec2::set_y(float value) { m_data[1] = value; }

Vec2::Vec2(float items[2])
{
    m_data[0] = items[0];
    m_data[1] = items[1];
}
Vec2::Vec2(float x, float y)
{
    m_data[0] = x;
    m_data[1] = y;
}

float Vec2::get_length() {
    return std::sqrt(get_dot(*this));
}
float Vec2::get_dot(Vec2 &b) {
    return m_data[0] * b.m_data[0] + m_data[1] * b.m_data[1];
}
Vec2 Vec2::get_normalized() {
    return (*this) / get_length();
}

Vec2& Vec2::do_normalize() {
    float length = get_length();
    (*this) /= length;
    return *this;
}
bool Vec2::read(BinaryStream &stream) {
    auto x = stream.read_float();
    if (!x.has_value()) return false;

    auto y = stream.read_float();
    if (!y.has_value()) return false;

    m_data[0] = x.value();
    m_data[1] = y.value();
    return true;
}

Vec2& Vec2::operator+=(const Vec2 &rhs)
{
    m_data[0] += rhs.m_data[0];
    m_data[1] += rhs.m_data[1];
    return *this;
}
Vec2& Vec2::operator-=(const Vec2 &rhs)
{
    m_data[0] -= rhs.m_data[0];
    m_data[1] -= rhs.m_data[1];
    return *this;
}
Vec2& Vec2::operator*=(float rhs)
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    return *this;
}
Vec2& Vec2::operator/=(float rhs)
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    return *this;
}