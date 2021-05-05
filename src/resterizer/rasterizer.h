//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H
#include "commonDefine.h"
#include "vector2.h"
#include "vector3.h"
#include "camera.h"

namespace rasterizer
{
class Rasterizer
{
public:
    enum class TopologyType : uint32_t
    {
        Unknowed = 0,
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points,
    };

public:
    Rasterizer();
    ~Rasterizer();
    explicit Rasterizer(const Rasterizer&) = delete;
    Rasterizer& operator=(const Rasterizer&) = delete;
    TopologyType topologyType() const;
    void setTopologyType(TopologyType type);
    void draw();
    void clear();
    /**
     * 判断当前点是否在三角形内部
     * @param point 当前点坐标
     * @param vertexes 三角形顶点坐标
     * @param barycentricCoord 中心坐标
     * @return
     */
    static bool insideTriangle(const Vector3& point, const std::vector<Vector3>& vertexes, std::vector<float>& barycentricCoord);
    inline Camera& refCamera();
    inline std::vector<float>& refBufferZ();
    inline std::vector<std::vector<float>>& refFramebuffer();

private:
    TopologyType m_topologyType = TopologyType::Unknowed;
    std::vector<Vector3> m_color;
    std::vector<Vector3> m_position;
    std::vector<Vector2> m_texCoord;
    std::vector<float> m_zBuffer;
    std::vector<std::vector<float>> m_framebuffer;
    Camera m_camera{};
};
} // namespace rasterizer

#include "rasterizer.inl"
#endif //RASTERIZER_RASTERIZER_H
