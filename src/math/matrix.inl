#include "matrix4.h"

namespace rasterizer
{

inline Vector3& operator*=(Vector3& v, const Matrix4& m)
{
    m.transformVector(&v);
    return v;
}

inline const Vector3 operator*(const Matrix4& m, const Vector3& v)
{
    Vector3 x;
    m.transformVector(v, &x);
    return x;
}

}