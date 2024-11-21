#include "vec4.h"
#include <cmath>
float Vec4::x() const { return m_data[0]; }
float Vec4::y() const { return m_data[1]; }
float Vec4::z() const { return m_data[2]; }
float Vec4::w() const { return m_data[3]; }
void Vec4::set_x(float value) { m_data[0] = value; }
void Vec4::set_y(float value) { m_data[1] = value; }
void Vec4::set_z(float value) { m_data[2] = value; }
void Vec4::set_w(float value) { m_data[3] = value; }

Vec4::Vec4(float items[4])
{
    m_data[0] = items[0];
    m_data[1] = items[1];
    m_data[2] = items[2];
    m_data[3] = items[3];
}
Vec4::Vec4(float x, float y, float z, float w) {
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
    m_data[3] = w;
}
Vec4::Vec4(Vec3 source) {
    m_data[0] = source.x();
    m_data[1] = source.y();
    m_data[2] = source.z();
    m_data[3] = 1.0f;
}

float Vec4::get_length() {
    return std::sqrt(get_dot(*this));
}
float Vec4::get_dot(Vec4 &b) {
    return m_data[0] * b.m_data[0] + m_data[1] * b.m_data[1] + m_data[2] * b.m_data[2] + m_data[3] * b.m_data[3];
}
Vec4 Vec4::get_normalized() {
    return (*this) / get_length();
}
Vec4& Vec4::do_normalize() {
    (*this) /= get_length();
    return *this;
}

Vec4& Vec4::operator+=(const Vec4 &rhs) {
    m_data[0] += rhs.m_data[0];
    m_data[1] += rhs.m_data[1];
    m_data[2] += rhs.m_data[2];
    m_data[3] += rhs.m_data[3];
    return *this;
}
Vec4& Vec4::operator-=(const Vec4 &rhs) {
    m_data[0] -= rhs.m_data[0];
    m_data[1] -= rhs.m_data[1];
    m_data[2] -= rhs.m_data[2];
    m_data[3] -= rhs.m_data[3];
    return *this;
}
Vec4& Vec4::operator*=(float rhs)  {
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    m_data[3] *= rhs;
    return *this;
}
Vec4& Vec4::operator/=(float rhs) {
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    m_data[3] /= rhs;
    return *this;
}

float const* Vec4::data()
{
    return m_data;
}

bool Vec4::read(BinaryStream &stream) {
    auto x = stream.read_float();
    if (!x.has_value()) return false;

    auto y = stream.read_float();
    if (!y.has_value()) return false;

    auto z = stream.read_float();
    if (!z.has_value()) return false;

    auto w = stream.read_float();
    if (!w.has_value()) return false;

    m_data[0] = x.value();
    m_data[1] = y.value();
    m_data[2] = z.value();
    m_data[3] = w.value();
    return true;
}
