#include "vector2.h"

namespace rasterizer
{
inline const Vector2 Vector2::operator+(const Vector2& v) const
{
    Vector2 result(*this);
    result.add(v);
    return result;
}

inline Vector2& Vector2::operator+=(const Vector2& v)
{
    add(v);
    return *this;
}

inline const Vector2 Vector2::operator-(const Vector2& v) const
{
    Vector2 result(*this);
    result.subtract(v);
    return result;
}

inline Vector2& Vector2::operator-=(const Vector2& v)
{
    subtract(v);
    return *this;
}

inline const Vector2 Vector2::operator-() const
{
    Vector2 result(*this);
    result.negate();
    return result;
}

inline const Vector2 Vector2::operator*(float x) const
{
    Vector2 result(*this);
    result.scale(x);
    return result;
}

inline Vector2& Vector2::operator*=(float x)
{
    scale(x);
    return *this;
}

inline const Vector2 Vector2::operator/(float x_) const
{
    return Vector2(x / x_, y / x_);
}

inline bool Vector2::operator<(const Vector2& v) const
{
    return (x == v.x) ? y < v.y : x < v.y;
}

inline bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

inline bool Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}

inline const Vector2 operator*(float x, const Vector2& v)
{
    Vector2 result(v);
    result.scale(x);
    return result;
}
} // namespace rasterizer