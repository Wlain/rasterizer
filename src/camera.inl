#include "camera.h"

namespace rasterizer
{
inline const Matrix4& Camera::getModelMatrix() const
{
    return m_modelMatrix;
}

inline const Matrix4& Camera::getViewMatrix() const
{
    return m_viewMatrix;
}

inline const Matrix4& Camera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}
} // namespace rasterizer