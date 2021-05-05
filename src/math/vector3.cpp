//
// Created by william on 2021/5/3.
//

#include "vector3.h"

#include "commonDefine.h"

namespace rasterizer
{
Vector3::Vector3() :
    x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x_, float y_, float z_) :
    x(x_), y(y_), z(z_)
{
}

Vector3::Vector3(const float* array)
{
    set(array);
}

Vector3::Vector3(const Vector3& p1, const Vector3& p2)
{
    set(p1, p2);
}

Vector3::Vector3(const Vector3& copy)
{
    set(copy);
}

Vector3::~Vector3() = default;

void Vector3::cross(const Vector3& v)
{
    cross(*this, v, this);
}

void Vector3::cross(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    RAS_ASSERT(dst);
    float x = (v1.y * v2.z) - (v1.z * v2.y);
    float y = (v1.z * v2.x) - (v1.x * v2.z);
    float z = (v1.x * v2.y) - (v1.y * v2.x);
    dst->x = x;
    dst->y = y;
    dst->z = z;
}

float Vector3::length() const
{
    return sqrt(lengthSquared());
}

float Vector3::lengthSquared() const
{
    return (x * x + y * y + z * z);
}

float Vector3::dot(const Vector3& v) const
{
    return dot(*this, v);
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

void Vector3::subtract(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3::subtract(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    RAS_ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

Vector3& Vector3::normalize()
{
    normalize(this);
    return *this;
}

void Vector3::normalize(Vector3* dst) const
{
    RAS_ASSERT(dst);

    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
        dst->z = z;
    }
    float n = length();
    if (n == 1.0f || n < FLT_MIN)
    {
        return;
    }
    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
    dst->z *= n;
}

void Vector3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::set(const float* array)
{
    RAS_ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
}

void Vector3::set(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::set(const Vector3& p1, const Vector3& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

} // namespace rasterizer