#include "rasterizer.h"

namespace rasterizer
{
inline Camera& Rasterizer::refCamera()
{
    return m_camera;
}

inline void Rasterizer::setTopologyType(TopologyType type)
{
    m_topologyType = type;
}

inline void Rasterizer::setFilePath(std::string_view path)
{
    m_filePath = path;
}

inline std::vector<float>& Rasterizer::refBufferZ()
{
    return m_zBuffer;
}

inline std::vector<std::vector<float>>& Rasterizer::refFramebuffer()
{
    return m_framebuffer;
}


inline std::vector<unsigned short>& Rasterizer::refIndices()
{
    return m_indices;
}

inline std::vector<std::array<unsigned char, 3>>& Rasterizer::refColor()
{
    return m_color;
}

inline std::vector<Vector3>& Rasterizer::refPosition()
{
    return m_position;
}

} // namespace rasterizer