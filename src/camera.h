//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_CAMERA_H
#define RASTERIZER_CAMERA_H
#include "matrix4.h"

namespace rasterizer
{
class Camera
{
public:
    Camera();
    ~Camera();
    inline const Matrix4& getModelMatrix() const;
    void setModelMatrix(const Matrix4& matrix);
    inline const Matrix4& getViewMatrix() const;
    void setViewMatrix(const Matrix4& matrix);
    inline const Matrix4& getProjectionMatrix() const;
    void setProjectionMatrix(const Matrix4& matrix);

private:
    mutable Matrix4 m_modelMatrix;
    mutable Matrix4 m_viewMatrix;
    mutable Matrix4 m_projectionMatrix;
};
} // namespace rasterizer

#include "camera.inl"
#endif //RASTERIZER_CAMERA_H
