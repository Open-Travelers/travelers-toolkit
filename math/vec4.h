#ifndef VEC4_H
#define VEC4_H

#include "../binary_stream.h"
#include "vec3.h"
class Vec4 {
public:
    Vec4(float items[4]);
    Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
    Vec4(Vec3 source);

public:
    float get_length();
    float get_dot(Vec4 &b);

    Vec4 get_normalized();
    Vec4 &do_normalize();


    float x() const;
    float y() const;
    float z() const;
    float w() const;

    void set_x(float value);
    void set_y(float value);
    void set_z(float value);
    void set_w(float value);
public:
    Vec4& operator+=(const Vec4 &rhs);
    Vec4& operator-=(const Vec4 &rhs);
    Vec4& operator*=(float rhs);
    Vec4& operator/=(float rhs);
    friend Vec4 operator+(Vec4 lhs, Vec4 const& rhs) {
        return Vec4(
            lhs.x() + rhs.x(),
            lhs.y() + rhs.y(),
            lhs.z() + rhs.z(),
            lhs.w() + rhs.w()
        );
    }
    friend Vec4 operator-(Vec4 lhs, const Vec4 &rhs) {
        return Vec4(
            lhs.x() - rhs.x(),
            lhs.y() - rhs.y(),
            lhs.z() - rhs.z(),
            lhs.w() - rhs.w()
        );
    }
    friend Vec4 operator*(Vec4 lhs, float rhs)
    {
        return Vec4(
            lhs.x() * rhs,
            lhs.y() * rhs,
            lhs.z() * rhs,
            lhs.w() * rhs
        );
    }
    friend Vec4 operator/(Vec4 lhs, float rhs)
    {
        return Vec4(
                lhs.x() / rhs,
                lhs.y() / rhs,
                lhs.z() / rhs,
                lhs.w() / rhs
        );
    }

public:
    float const* data();
public:
    bool read(BinaryStream &stream);
protected:
    float m_data[4];
};

#endif
