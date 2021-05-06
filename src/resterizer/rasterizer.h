//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H
#include "camera.h"
#include "commonDefine.h"
#include "vector2.h"
#include "vector3.h"

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
    void initWithFrameBufferSize(int width, int height);
    inline void setTopologyType(TopologyType type);
    void setGeometricTransform();
    inline void setFilePath(std::string_view path);
    /**
     * 1.判断每个像素点是否在屏幕对应的三角形中。
     * 2.如果在三角形中，计算其三角形重心坐标，并进行顶点属性插值
     * 3.将颜色信息写入frameBuffer
     * */
    void draw();
    void clear();
    void saveResult();
    inline Camera& refCamera();
    inline std::vector<float>& refBufferZ();
    inline std::vector<unsigned short>& refIndices();
    inline std::vector<std::array<unsigned char, 3>>& refColor();
    inline std::vector<Vector3>& refPosition();
    inline std::vector<std::vector<float>>& refFramebuffer();

    /**
     * 判断当前点是否在三角形内部
     * @param point 当前点坐标
     * @param vertexes 三角形顶点坐标
     * @param barycentricCoord 中心坐标
     * @return
     */
    static bool insideTriangle(const Vector3& point, const std::vector<Vector3>& vertexes, std::vector<float>& barycentricCoord);

private:
    TopologyType m_topologyType = TopologyType::Unknowed;
    std::vector<std::array<unsigned char, 3>> m_color;
    std::vector<Vector3> m_position;
    std::vector<Vector2> m_texCoord;
    std::vector<unsigned short> m_indices;
    std::vector<float> m_zBuffer;
    std::vector<std::vector<float>> m_framebuffer;
    Camera m_camera{};
    int m_framebufferWidth = 0;
    int m_framebufferHeight = 0;
    std::string_view m_filePath;
    bool m_enableMSAA = false;
};
} // namespace rasterizer

#include "rasterizer.inl"
#endif //RASTERIZER_RASTERIZER_H
