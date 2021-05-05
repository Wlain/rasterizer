//
// Created by william on 2021/5/4.
//

#include "camera.h"

namespace rasterizer
{
Camera::Camera() = default;
Camera::~Camera() = default;

void Camera::setModelMatrix(const Matrix4& matrix)
{
}

void Camera::setViewMatrix(const Matrix4& matrix)
{
}

const Matrix4& Camera::getViewProjectionMatrix() const
{
    static Matrix4 m;
    return m;
}

void Camera::setProjectionMatrix(const Matrix4& matrix)
{
}
}