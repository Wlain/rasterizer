#include "rasterizer.h"

namespace rasterizer
{
inline Camera& Rasterizer::refCamera()
{
    return m_camera;
}

inline std::vector<float>& Rasterizer::refBufferZ()
{
    return m_zBuffer;
}

inline std::vector<std::vector<float>>& Rasterizer::refFramebuffer()
{
    return m_framebuffer;
}

} // namespace rasterizer