#include "mat4x4.h"
#include <cmath>
Mat4x4::Mat4x4()
{
    for (int i = 0; i < 16; i++)
        m_data[i] = 0;
}
Mat4x4::Mat4x4(float array[16])
{
    for (int i = 0; i < 16; i++)
        m_data[i] = array[i];
}
Mat4x4::Mat4x4(Vec4 rows[4])
{
    for (int i = 0; i < 4; i++)
    {
        m_data[4 * i + 0] = rows[i].x();
        m_data[4 * i + 1] = rows[i].y();
        m_data[4 * i + 2] = rows[i].z();
        m_data[4 * i + 3] = rows[i].w();
    }
}
Mat4x4 Mat4x4::Translate(Vec3 position)
{
    const float data[] = {
        1, 0, 0, position.x(),
        0, 1, 0, position.y(),
        0, 0, 1, position.z(),
        0, 0, 0, 1
    };
    return Mat4x4((float*) data);
}
Mat4x4 Mat4x4::Scale(Vec3 factors)
{
    const float data[] = {
        factors.x(), 0, 0, 0,
        0, factors.y(), 0, 0, 
        0, 0, factors.z(), 0,
        0, 0, 0, 1
    };
    return Mat4x4((float*) data);
}
Vec3 Mat4x4::get_scale_vector()
{
    return Vec3(m_data[0], m_data[5], m_data[10]);
}
Mat4x4 Mat4x4::Identity()
{
    static const float data[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return Mat4x4((float*) data);
}
Mat4x4 Mat4x4::LookAt(Vec3 from, Vec3 to, Vec3 up)
{
    auto zvec = (from - to).get_normalized();
    auto xvec = up.get_cross(zvec).get_normalized();
    auto yvec = zvec.get_cross(xvec);

    const float data[] = {
        xvec.x(), yvec.x(), -zvec.x(), 0,
        xvec.y(), yvec.y(), -zvec.y(), 0,
        xvec.z(), yvec.z(), -zvec.z(), 0,
        -from.get_dot(xvec), -from.get_dot(yvec), from.get_dot(zvec), 1
    };
    return Mat4x4((float*)data);
}
Mat4x4 Mat4x4::RotateX(float angle)
{
    const float data[] =  {
        1, 0, 0, 0,
        0, cosf(angle), -sinf(angle), 0,
        0, sinf(angle), cosf(angle), 0,
        0, 0, 0, 1
    };
    return Mat4x4((float*)data);
}
Mat4x4 Mat4x4::RotateY(float angle)
{
    const float data[] =  {
        cosf(angle), 0, sinf(angle), 0,
        0, 1, 0, 0,
        -sinf(angle), 0, cosf(angle), 0,
        0, 0, 0, 1
    };
    return Mat4x4((float*)data);
}
Mat4x4 Mat4x4::RotateZ(float angle)
{
    const float data[] = {
        cosf(angle), -sinf(angle), 0, 0,
        sinf(angle), cosf(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return Mat4x4((float*)data);
}

Mat4x4 Mat4x4::PerspectiveProjection(float aspect, float fov, float near, float far)
{
    /*const float data[] = {
        1/(aspect * tanf(fov / 2)), 0, 0, 0,
        0, 1/tanf(fov/2), 0, 0,
        0, 0, -(far + near)/(far - near), -1,
        0, 0, -2*((far*near)/(far-near)), 0
    };*/

    float f, fn;
    Mat4x4 dest;
    f  = 1.0f / tanf(fov * 0.5f);
    fn = 1.0f / (near - far);

    dest.set(0, 0, f / aspect);
    dest.set(1, 1, f);
    dest.set(2, 2, -(near + far) * fn);
    dest.set(3, 2, 1.0f);
    dest.set(2, 3, 2.0f * near * far * fn);
    return dest;
}

/*
Mat4x4 Mat4x4::PerspectiveProjection(float aspect, float fov, float near, float far)
{
    float tan_half_fov_y = 1.f / tanf(fov / 2);
    float top = near * tan_half_fov_y;
    float right = top * aspect;
    const float data[] = {
        near / right, 0, 0, 0,
        0, near / top, 0, 0,
        0, 0, -(far + near)/(far - near), -1,
        0, 0, -2*((far*near)/(far-near)), 0
    };
    return Mat4x4((float*) data);
}*/
float Mat4x4::get(int i) {
    if (i < 0 || i >= 16)
        throw std::invalid_argument("Index");
    return m_data[i];
}
float Mat4x4::get(int x, int y) {
    if (x < 0 || x >= 4)
        throw std::invalid_argument("X");
    if (y < 0 || y >= 4)
        throw std::invalid_argument("Y");
    return m_data[x + y * 4];
}
void Mat4x4::set(int i, float value) {
    if (i < 0 || i >= 16)
        throw std::invalid_argument("Index");
    m_data[i] = value;
}
void Mat4x4::set(int x, int y, float value) {
    if (x < 0 || x >= 4)
        throw std::invalid_argument("X");
    if (y < 0 || y >= 4)
        throw std::invalid_argument("Y");
    m_data[x + y * 4] = value;

}

Vec3 Mat4x4::transform_vector(Vec3 const& source)
{
    return Vec3(
        m_data[0] * source.x() + m_data[1] * source.y() + m_data[2] * source.z() + m_data[3],
        m_data[4] * source.x() + m_data[5] * source.y() + m_data[6] * source.z() + m_data[7],
        m_data[8] * source.x() + m_data[9] * source.y() + m_data[10] * source.z() + m_data[11]
    );
}
Vec4 Mat4x4::transform_vector(Vec4 const& source)
{
    return Vec4(
        m_data[0] * source.x() + m_data[1] * source.y() + m_data[2] * source.z() + m_data[3] * source.w(),
        m_data[4] * source.x() + m_data[5] * source.y() + m_data[6] * source.z() + m_data[7] * source.w(),
        m_data[8] * source.x() + m_data[9] * source.y() + m_data[10] * source.z() + m_data[11] * source.w(),
        m_data[12] * source.x() + m_data[13] * source.y() + m_data[14] * source.z() + m_data[15] * source.w()
    );
}
Vec4 Mat4x4::get_row(int n)
{
    return Vec4(
        m_data[4*n],
        m_data[4*n + 1],
        m_data[4*n + 2],
        m_data[4*n + 3]
    );
}
Vec4 Mat4x4::get_column(int n)
{
    return Vec4(
        m_data[4*n],
        m_data[4*(n + 1)],
        m_data[4*(n + 2)],
        m_data[4*(n + 3)]
    );
}

Mat4x4 Mat4x4::get_inverse()
{
    throw std::logic_error("get_inverse() not yet implemented.");
}
Mat4x4& Mat4x4::do_invert()
{
    throw std::logic_error("do_invert() not yet implemented.");
}

Mat4x4 Mat4x4::get_transposed()
{
    auto ret = Mat4x4();
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            ret.set(x, y, get(y, x));
        }
    }
    return ret;
}
Mat4x4 &Mat4x4::do_transpose()
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            float a = get(x, y);
            set(x, y, get(y, x));
            set(y, x, a);

        }
    }
    return *this;
}
bool Mat4x4::read(BinaryStream &stream)
{
    /// TODO: this is slow as bwals
    float data[16];
    for (int i = 0; i < 16; i++)
    {
        auto val = stream.read_float();
        if (!val.has_value()) return false;
        data[i] = val.value();
    }

    for (int i = 0; i < 16; i++)
        set(i, data[i]);
    return true;
}