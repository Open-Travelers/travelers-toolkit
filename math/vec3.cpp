#include "vec3.h"
#include <cmath>
float Vec3::x() const { return m_data[0]; }
float Vec3::y() const { return m_data[1]; }
float Vec3::z() const { return m_data[2]; }
void Vec3::set_x(float value) { m_data[0] = value; }
void Vec3::set_y(float value) { m_data[1] = value; }
void Vec3::set_z(float value) { m_data[2] = value; }

Vec3::Vec3(float items[3])
{
    m_data[0] = items[0];
    m_data[1] = items[1];
    m_data[2] = items[2];
}
Vec3::Vec3(float x, float y, float z)
{
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
}
Vec3::Vec3(float length, Vec3 direction)
{
    m_data[0] = direction.x() * length;
    m_data[1] = direction.y() * length;
    m_data[2] = direction.z() * length;
}

float Vec3::get_length() {
    return std::sqrt(get_dot(*this));
}
float Vec3::get_dot(Vec3 &b) {
    return m_data[0] * b.m_data[0] + m_data[1] * b.m_data[1] + m_data[2] * b.m_data[2];
}
Vec3 Vec3::get_cross(Vec3 &b) {
    return Vec3(y() * b.z() - z() * b.y(), z() * b.x() - x() * b.z(), x() * b.y() - y() * b.x());
}
Vec3 Vec3::get_normalized() {
    return (*this) / get_length();
}

Vec3& Vec3::do_cross(Vec3 &b) {
    float vx = y() * b.z() - z() * b.y();
    float vy = z() * b.x() - x() * b.z();
    float vz = x() * b.y() - y() * b.x();
    m_data[0] = vx;
    m_data[1] = vy;
    m_data[2] = vz;
    return *this;
}
Vec3& Vec3::do_normalize() {
    (*this) /= get_length();
    return *this;
}
bool Vec3::read(BinaryStream &stream) {
    auto x = stream.read_float();
    if (!x.has_value()) return false;

    auto y = stream.read_float();
    if (!y.has_value()) return false;

    auto z = stream.read_float();
    if (!z.has_value()) return false;

    m_data[0] = x.value();
    m_data[1] = y.value();
    m_data[2] = z.value();
    return true;
}

Vec3& Vec3::operator+=(const Vec3 &rhs)
{
    m_data[0] += rhs.m_data[0];
    m_data[1] += rhs.m_data[1];
    m_data[2] += rhs.m_data[2];
    return *this;
}
Vec3& Vec3::operator-=(const Vec3 &rhs)
{
    m_data[0] -= rhs.m_data[0];
    m_data[1] -= rhs.m_data[1];
    m_data[2] -= rhs.m_data[2];
    return *this;
}
Vec3& Vec3::operator*=(float rhs)
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    return *this;
}
Vec3& Vec3::operator/=(float rhs)
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    return *this;
}