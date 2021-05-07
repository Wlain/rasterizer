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
    m_modelMatrix = matrix;
}

void Camera::setViewMatrix(const Matrix4& matrix)
{
    m_viewMatrix = matrix;
}

void Camera::setProjectionMatrix(const Matrix4& matrix)
{
    m_projectionMatrix = matrix;
}
} // namespace rasterizer