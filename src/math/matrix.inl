#include "matrix4.h"

namespace rasterizer
{

inline Vector4& operator*=(Vector4& v, const Matrix4& m)
{
    m.transformVector(&v);
    return v;
}

inline const Vector4 operator*(const Matrix4& m, const Vector4& v)
{
    Vector4 x;
    m.transformVector(v, &x);
    return x;
}

}