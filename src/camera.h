//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_CAMERA_H
#define RASTERIZER_CAMERA_H
#include "math/matrix4.h"
#include "math/vector2.h"

namespace rasterizer
{
class Camera
{
public:
    Camera();
    ~Camera();
    const Matrix4& getModelMatrix() const;
    void setModelMatrix(const Matrix4& matrix);
    const Matrix4& getViewMatrix() const;
    void setViewMatrix(const Matrix4& matrix);
    const Matrix4& getProjectionMatrix() const;
    void setProjectionMatrix(const Matrix4& matrix);
    const Matrix4& getViewProjectionMatrix() const;

private:
    mutable Matrix4 m_view;
    mutable Matrix4 m_projection;
    mutable Matrix4 m_viewProjection;
};
} // namespace rasterizer

#endif //RASTERIZER_CAMERA_H
