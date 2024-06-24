#ifndef VEC2_H
#define VEC2_H

#include "../binary_stream.h"

class Vec2 {
public:
    Vec2(float items[2]);
    Vec2(float x, float y);

public:
    float get_length();
    float get_dot(Vec2 &b);
    
    Vec2 get_normalized();
    Vec2 &do_normalize();

    float x() const;
    float y() const;

    void set_x(float value);
    void set_y(float value);
public:
    Vec2& operator+=(const Vec2 &rhs);
    Vec2& operator-=(const Vec2 &rhs);
    Vec2& operator*=(float rhs);
    Vec2& operator/=(float rhs);

    friend Vec2 operator+(Vec2 lhs, const Vec2 &rhs) {
        return Vec2(
                lhs.x() + rhs.x(),
                lhs.y() + rhs.y()
        );
    }
    friend Vec2 operator-(Vec2 lhs, const Vec2 &rhs) {
        return Vec2(
                lhs.x() - rhs.x(),
                lhs.y() - rhs.y()
        );
    }
    friend Vec2 operator*(Vec2 lhs, float rhs) {
        return Vec2(
                lhs.x() * rhs,
                lhs.y() * rhs
        );
    }
    friend Vec2 operator/(Vec2 lhs, float rhs) {
        return Vec2(
                lhs.x() / rhs,
                lhs.y() / rhs
        );
    }

public:
    float *data();
public:
    bool read(BinaryStream &stream);
protected:
    float m_data[2];
};

#endif
