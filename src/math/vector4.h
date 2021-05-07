//
// Created by william on 2021/5/5.
//

#ifndef RASTERIZER_VECTOR4_H
#define RASTERIZER_VECTOR4_H
namespace rasterizer
{
class Vector4
{
public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    ~Vector4();
    void perspectiveDivision();

public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};
} // namespace rasterizer

#endif //RASTERIZER_VECTOR4_H
