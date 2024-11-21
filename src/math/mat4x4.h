#ifndef MAT4X4_H
#define MAT4X4_H

#include "vec3.h"
#include "vec4.h"

class Mat4x4
{
public:
    Mat4x4();
    Mat4x4(float array[16]);
    Mat4x4(Vec4 rows[4]);
public:
    static Mat4x4 Identity();
    static Mat4x4 Translate(Vec3 position);
    static Mat4x4 LookAt(Vec3 from, Vec3 to, Vec3 up);
    static Mat4x4 RotateX(float angle);
    static Mat4x4 RotateY(float angle);
    static Mat4x4 RotateZ(float angle);
    static Mat4x4 Scale(Vec3 factors);
    static Mat4x4 PerspectiveProjection(float aspect, float fov, float near, float far);

public:
    Vec3 transform_vector(Vec3 const& source);
    Vec4 transform_vector(Vec4 const& source);
    Vec3 get_scale_vector();
    Vec3 get_translation_vector();
public:
    bool read(BinaryStream &stream);

public:
    Vec4 get_row(int n);
    Vec4 get_column(int n);

    Mat4x4 get_inverse();
    Mat4x4& do_invert();

    Mat4x4 get_transposed();
    Mat4x4 &do_transpose();

    float const* data() { return m_data; }
    float get(int i);
    float get(int x, int y);
    void set(int i, float value);
    void set(int x, int y, float value);
public:
    float operator[](int idx) const {
        return m_data[idx];
    }

    friend Mat4x4 operator*(Mat4x4 lhs, float rhs)
    {
        Mat4x4 mat = lhs;
        for (int i = 0; i < 16; i++)
            mat.m_data[i] *= rhs;
        return mat;
    }
    friend Mat4x4 operator*(Mat4x4 m1, Mat4x4 const& m2)
    {
        float dest[16] = {0};
        float a00 = m1[0 * 4 + 0], a01 = m1[0 * 4 + 1], a02 = m1[0 * 4 + 2], a03 = m1[0 * 4 + 3],
              a10 = m1[1 * 4 + 0], a11 = m1[1 * 4 + 1], a12 = m1[1 * 4 + 2], a13 = m1[1 * 4 + 3],
              a20 = m1[2 * 4 + 0], a21 = m1[2 * 4 + 1], a22 = m1[2 * 4 + 2], a23 = m1[2 * 4 + 3],
              a30 = m1[3 * 4 + 0], a31 = m1[3 * 4 + 1], a32 = m1[3 * 4 + 2], a33 = m1[3 * 4 + 3],
    
              b00 = m2[0 * 4 + 0], b01 = m2[0 * 4 + 1], b02 = m2[0 * 4 + 2], b03 = m2[0 * 4 + 3],
              b10 = m2[1 * 4 + 0], b11 = m2[1 * 4 + 1], b12 = m2[1 * 4 + 2], b13 = m2[1 * 4 + 3],
              b20 = m2[2 * 4 + 0], b21 = m2[2 * 4 + 1], b22 = m2[2 * 4 + 2], b23 = m2[2 * 4 + 3],
              b30 = m2[3 * 4 + 0], b31 = m2[3 * 4 + 1], b32 = m2[3 * 4 + 2], b33 = m2[3 * 4 + 3];

        dest[0*4 + 0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
        dest[0*4 + 1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
        dest[0*4 + 2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
        dest[0*4 + 3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
        dest[1*4 + 0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
        dest[1*4 + 1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
        dest[1*4 + 2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
        dest[1*4 + 3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
        dest[2*4 + 0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
        dest[2*4 + 1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
        dest[2*4 + 2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
        dest[2*4 + 3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
        dest[3*4 + 0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
        dest[3*4 + 1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
        dest[3*4 + 2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
        dest[3*4 + 3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
        return Mat4x4(dest);
    }
    

protected:
    float m_data[16];
};
#endif
