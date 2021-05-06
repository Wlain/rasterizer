//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H
#include "camera.h"
#include "commonDefine.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace rasterizer
{
class Rasterizer
{
public:
    enum class Primitive : uint32_t
    {
        Triangles = 0,
        TriangleStrip,
        Lines,
        LineStrip,
        Points,
    };

    enum class Buffers
    {
        Color = 1,
        Depth
    };

public:
    Rasterizer();
    ~Rasterizer();
    explicit Rasterizer(const Rasterizer&) = delete;
    Rasterizer& operator=(const Rasterizer&) = delete;
    inline Camera& refCamera();
    inline std::vector<float>& refBufferZ();
    inline std::vector<unsigned short>& refIndices();
    inline std::vector<std::array<unsigned char, 3>>& refColor();
    inline std::vector<Vector4>& refPosition();
    inline std::vector<std::vector<float>>& refFramebuffer();
    void initWithFrameBufferSize(int width, int height);
    inline Primitive topologyType() const;
    inline void setTopologyType(Primitive type);
    void setGeometricTransform();
    inline void setFilePath(std::string_view path);
    /**
     * 1.判断每个像素点是否在屏幕对应的三角形中。
     * 2.如果在三角形中，计算其三角形重心坐标，并进行顶点属性插值
     * 3.将颜色信息写入frameBuffer
     * */
    void drawTriangles();
    void clear(Buffers buffers);
    void saveResult();

private:
    Primitive m_topologyType = Primitive::Triangles;
    std::vector<std::array<unsigned char, 3>> m_color;
    std::vector<Vector4> m_position;
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

inline Rasterizer::Buffers operator|(Rasterizer::Buffers a, Rasterizer::Buffers b);
inline Rasterizer::Buffers operator&(Rasterizer::Buffers a, Rasterizer::Buffers b);

} // namespace rasterizer

#include "rasterizer.inl"
#endif //RASTERIZER_RASTERIZER_H
