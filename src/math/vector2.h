//
// Created by william on 2021/5/3.
//

#ifndef RASTERIZER_VECTOR2_H
#define RASTERIZER_VECTOR2_H

namespace rasterizer
{
/// Defines a 2-element floating point vector.
class Vector2
{
public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& copy);
    Vector2(const float* array);
    /// point
    Vector2(const Vector2& p1, const Vector2& p2);
    ~Vector2();
    static const Vector2& zero();
    static const Vector2& one();
    static const Vector2& unitX();
    static const Vector2& unitY();
    bool isZero() const;
    bool isOne() const;
    static float angle(const Vector2& v1, const Vector2& v2);
    void add(const Vector2& v);
    static void add(const Vector2& v1, const Vector2& v2, Vector2* dst);
    void clamp(const Vector2& min, const Vector2& max);
    static void clamp(const Vector2& v, const Vector2& min, const Vector2& max, Vector2* dst);
    float distance(const Vector2& v) const;
    float distanceSquared(const Vector2& v) const;
    float dot(const Vector2& v) const;
    static float dot(const Vector2& v1, const Vector2& v2);
    float cross(const Vector2& v) const;
    float length() const;
    float lengthSquared() const;
    void negate();
    Vector2& normalize();
    void normalize(Vector2* dst) const;
    void scale(float scalar);
    void scale(const Vector2& scale);
    void rotate(const Vector2& point, float angle);
    void set(float x, float y);
    void set(const float* array);
    void set(const Vector2& v);
    /// point
    void set(const Vector2& p1, const Vector2& p2);
    /// v1 - v2
    void subtract(const Vector2& v);
    /// v1 - v2
    static void subtract(const Vector2& v1, const Vector2& v2, Vector2* dst);
    void smooth(const Vector2& target, float elapsedTime, float responseTime);
    inline const Vector2 operator+(const Vector2& v) const;
    inline Vector2& operator+=(const Vector2& v);
    inline const Vector2 operator-(const Vector2& v) const;
    inline Vector2& operator-=(const Vector2& v);
    inline const Vector2 operator-() const;
    inline const Vector2 operator*(float x) const;
    inline Vector2& operator*=(float x);
    inline const Vector2 operator/(float x) const;
    inline bool operator<(const Vector2& v) const;
    inline bool operator==(const Vector2& v) const;
    inline bool operator!=(const Vector2& v) const;

public:
    float x = 0.0f;
    float y = 0.0f;
};

inline const Vector2 operator*(float x, const Vector2& v);

} // namespace rasterizer

#include "vector2.inl"
#endif //RASTERIZER_VECTOR2_H
