#ifndef VEC3_H
#define VEC3_H

#include "../binary_stream.h"

class Vec3 {
public:
    Vec3(float items[3]);
    Vec3(float x, float y, float z);
    Vec3(float length, Vec3 direction);

public:
    float get_length();
    float get_dot(Vec3 &b);
    Vec3 get_cross(Vec3 &b);
    Vec3 get_normalized();

    Vec3 &do_cross(Vec3 &b);
    Vec3 &do_normalize();

    float x() const;
    float y() const;
    float z() const;

    void set_x(float value);
    void set_y(float value);
    void set_z(float value);
public:
    Vec3& operator+=(const Vec3 &rhs);
    Vec3& operator-=(const Vec3 &rhs);
    Vec3& operator*=(float rhs);
    Vec3& operator/=(float rhs);

    friend Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
        return Vec3(
                lhs.x() + rhs.x(),
                lhs.y() + rhs.y(),
                lhs.z() + rhs.z()
        );
    }
    friend Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
        return Vec3(
                lhs.x() - rhs.x(),
                lhs.y() - rhs.y(),
                lhs.z() - rhs.z()
        );
    }
    friend Vec3 operator*(Vec3 lhs, float rhs) {
        return Vec3(
                lhs.x() * rhs,
                lhs.y() * rhs,
                lhs.z() * rhs
        );
    }
    friend Vec3 operator/(Vec3 lhs, float rhs) {
        return Vec3(
                lhs.x() / rhs,
                lhs.y() / rhs,
                lhs.z() / rhs
        );
    }

public:
    float *data();
public:
    bool read(BinaryStream &stream);
protected:
    float m_data[3];
};

#endif
