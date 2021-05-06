//
// Created by william on 2021/5/3.
//

#ifndef RASTERIZER_VECTOR3_H
#define RASTERIZER_VECTOR3_H
namespace rasterizer
{
class Vector3
{
public:
    Vector3();
    Vector3(float x, float y, float z);
    explicit Vector3(const float* array);
    Vector3(const Vector3& p1, const Vector3& p2);
    Vector3(const Vector3& copy);
    ~Vector3();
    static const Vector3& zero();
    static const Vector3& one();
    static const Vector3& unitX();
    static const Vector3& unitY();
    static const Vector3& unitZ();
    bool isZero() const;
    bool isOne() const;
    static float angle(const Vector3& v1, const Vector3& v2);
    void add(const Vector3& v);
    static void add(const Vector3& v1, const Vector3& v2, Vector3* dst);
    void clamp(const Vector3& min, const Vector3& max);
    static void clamp(const Vector3& min, const Vector3& max, Vector3* dst);
    void cross(const Vector3& v);
    static void cross(const Vector3& v1, const Vector3& v2, Vector3* dst);
    float distance(const Vector3& v) const;
    float distanceSquard(const Vector3& v) const;
    float dot(const Vector3& v) const;
    static float dot(const Vector3& v1, const Vector3& v2);
    float length() const;
    float lengthSquared() const;
    void negate();
    Vector3& normalize();
    void normalize(Vector3* dst) const;
    void scale(float scalar);
    void set(float x, float y, float z);
    void set(const float* array);
    void set(const Vector3& v);
    void set(const Vector3& p1, const Vector3& p2);
    void subtract(const Vector3& v);
    static void subtract(const Vector3& v1, const Vector3& v2, Vector3* dst);
    void smooth(const Vector3& target, float elapsedTime, float responseTime);
    inline Vector3 operator+(const Vector3& v) const;
    inline Vector3& operator+=(const Vector3& v);
    inline Vector3 operator-(const Vector3& v) const;
    inline Vector3& operator-=(const Vector3& v);
    inline Vector3 operator-() const;
    inline Vector3 operator*(float v) const;
    inline Vector3& operator*=(const Vector3& v);
    inline Vector3 operator/(float x) const;
    inline bool operator<(const Vector3& v) const;
    inline bool operator==(const Vector3& v) const;
    inline bool operator!=(const Vector3& v) const;

public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

inline Vector3 operator*(float x, const Vector3& v);
} // namespace rasterizer

#include "vector3.inl"
#endif //RASTERIZER_VECTOR3_H
