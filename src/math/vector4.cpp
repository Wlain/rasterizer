//
// Created by william on 2021/5/5.
//

#include "vector4.h"
namespace rasterizer
{
Vector4::Vector4() :
    x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Vector4::Vector4(float x_, float y_, float z_, float w_) :
    x(x_), y(y_), z(z_), w(w_)
{
}

void Vector4::perspectiveDivision()
{
    x /= w;
    y /= w;
    y /= w;
    w = 1.0f;
}

Vector4::~Vector4() = default;
} // namespace rasterizer