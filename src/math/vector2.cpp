//
// Created by william on 2021/5/3.
//
#include "vector2.h"

#include "common/commonDefine.h"

namespace rasterizer
{
Vector2::Vector2() :
    x(0.0f), y(0.0f)
{
}

Vector2::Vector2(float x, float y) :
    x(x), y(y)
{
}

Vector2::Vector2(const Vector2& copy)
{
    x = copy.x;
    y = copy.y;
}

Vector2::~Vector2() = default;

Vector2::Vector2(const float* array)
{
    set(array);
}

Vector2::Vector2(const Vector2& p1, const Vector2& p2)
{
    set(p1, p2);
}

const Vector2& Vector2::zero()
{
    static Vector2 v(0.0f, 0.0f);
    return v;
}

const Vector2& Vector2::one()
{
    static Vector2 v(1.0f, 1.0f);
    return v;
}

const Vector2& Vector2::unitX()
{
    static Vector2 v(1.0f, 0.0f);
    return v;
}

const Vector2& Vector2::unitY()
{
    static Vector2 v(0.0f, 1.0f);
    return v;
}

bool Vector2::isZero() const
{
    return x == 0.0f && y == 0.0f;
}

bool Vector2::isOne() const
{
    return x == 1.0f && y == 1.0f;
}

float Vector2::angle(const Vector2& v1, const Vector2& v2)
{
    float dz = v1.x * v2.y - v1.y * v2.x;
    return atan2f(fabsf(dz) + FLT_MIN, dot(v1, v2));
}

void Vector2::add(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

void Vector2::add(const Vector2& v1, const Vector2& v2, Vector2* dst)
{
    RAS_ASSERT(dst);
    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
}

void Vector2::clamp(const Vector2& min, const Vector2& max)
{
    RAS_ASSERT(!(min.x > max.x || min.y > max.y));
    std::clamp(x, min.x, min.x);
    std::clamp(y, min.y, min.y);
}

void Vector2::clamp(const Vector2& v, const Vector2& min, const Vector2& max, Vector2* dst)
{
    RAS_ASSERT(dst);
    RAS_ASSERT(!(min.x > max.x || min.y > max.y));
    std::clamp(dst->x, min.x, min.x);
    std::clamp(dst->y, min.y, min.y);
}

float Vector2::distance(const Vector2& v) const
{
    return sqrt(distanceSquared(v));
}

float Vector2::distanceSquared(const Vector2& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    return dx * dx + dy + dy;
}

float Vector2::dot(const Vector2& v) const
{
    return dot(*this, v);
}

float Vector2::dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::length() const
{
    return sqrt(lengthSquared());
}

float Vector2::lengthSquared() const
{
    return (x * x + y * y);
}

void Vector2::negate()
{
    x = -x;
    y = -y;
}

Vector2& Vector2::normalize()
{
    normalize(this);
    return *this;
}

void Vector2::normalize(Vector2* dst) const
{
    RAS_ASSERT(dst);
    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
    }
    float n = length();
    if (n == 1.0f || n < FLT_MIN)
    {
        return;
    }
    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
}

void Vector2::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vector2::scale(const Vector2& scale)
{
    x *= scale.x;
    y *= scale.y;
}

void Vector2::rotate(const Vector2& point, float angle)
{
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);
    if (point.isZero())
    {
        float tempX = x * cosAngle - y * sinAngle;
        y = y * cosAngle + x * sinAngle;
        x = tempX;
    }
    else
    {
        float tempX = x - point.x;
        float tempY = y - point.y;
        x = tempX * cosAngle - tempY * sinAngle + point.x;
        y = tempY * cosAngle + tempX * sinAngle + point.y;
    }
}

void Vector2::set(float x_, float y_)
{
    x = x_;
    y = y_;
}

void Vector2::set(const float* array)
{
    RAS_ASSERT(array);
    x = array[0];
    y = array[1];
}

void Vector2::set(const Vector2& v)
{
    x = v.x;
    y = v.y;
}

void Vector2::set(const Vector2& p1, const Vector2& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
}

void Vector2::subtract(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

void Vector2::subtract(const Vector2& v1, const Vector2& v2, Vector2* dst)
{
    RAS_ASSERT(dst);
    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
}

void Vector2::smooth(const Vector2& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

float Vector2::cross(const Vector2& v) const
{
    return y * v.x - x * v.y;
}
} // namespace rasterizer