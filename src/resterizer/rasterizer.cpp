//
// Created by william on 2021/5/4.
//

#include "rasterizer.h"

#include "commonDefine.h"

namespace rasterizer
{
Rasterizer::Rasterizer() = default;

Rasterizer::~Rasterizer() = default;

void Rasterizer::initWithFrameBufferSize(int width, int height)
{
    m_framebufferWidth = width;
    m_framebufferHeight = height;
    /// 初始化Buffer，FrameBuffer
    size_t bufferSize = m_framebufferWidth * m_framebufferHeight;
    m_zBuffer.resize(bufferSize, std::numeric_limits<float>::infinity());
    m_framebuffer.resize(bufferSize, std::vector<float>(4, 0.0f));
}

void Rasterizer::setGeometricTransform()
{
    for (auto& v : m_position)
    {
        /// 模型变换 -> 世界坐标系
        v = m_camera.getModelMatrix() * v;
        /// 视图变换 -> 观察者坐标系
        v = m_camera.getViewMatrix() * v;
        /// 投影变换 -> 裁剪坐标系
        v = m_camera.getProjectionMatrix() * v;
        /// 透视除法 -> 规范化坐标系
        v.perspectiveDivision();
        /// 视口变换 -> 屏幕坐标系
        v.x = (v.x + 1.0f) * (float)m_framebufferWidth * 0.5f;
        v.y = (1.0f - v.y) * (float)m_framebufferHeight * 0.5f;
        /**
        * 视口变换的详细步骤：
        * 1.丢弃z值
        * 2.缩放
        * 3.平移
        * */
    }
}

bool Rasterizer::insideTriangle(const Vector3& point, const std::vector<Vector4>& vertexes, std::vector<float>& barycentricCoord)
{
    RAS_ASSERT(vertexes.size() == 3);
    Vector2 abVector(vertexes[1].x - vertexes[0].x, vertexes[1].y - vertexes[0].y);
    Vector2 bcVector(vertexes[2].x - vertexes[1].x, vertexes[2].y - vertexes[1].y);
    Vector2 caVector(vertexes[0].x - vertexes[2].x, vertexes[0].y - vertexes[2].y);

    Vector2 apVector(point.x - vertexes[0].x, point.y - vertexes[0].y);
    Vector2 bpVector(point.x - vertexes[1].x, point.y - vertexes[1].y);
    Vector2 cpVector(point.x - vertexes[2].x, point.y - vertexes[2].y);

    float result[3];
    /// a * b = |a| * |b| * sin(theta)
    result[0] = abVector.cross(apVector);
    result[1] = bcVector.cross(bpVector);
    result[2] = caVector.cross(cpVector);

    if ((result[0] > 0.0f && result[1] > 0.0f && result[2] > 0.0f) || (result[0] < 0.0f && result[1] < 0.0f && result[2] < 0.0f))
    {
        /// 计算重心坐标系
        float triangleArea = std::fabsf(abVector.cross(bcVector));
        barycentricCoord.push_back(abs(result[0] / triangleArea));
        barycentricCoord.push_back(abs(result[1] / triangleArea));
        barycentricCoord.push_back(abs(result[2] / triangleArea));
        return true;
    }
    return false;
}

void Rasterizer::drawTriangles()
{
    /// 开始遍历像素
    for (int row = 0; row < m_framebufferHeight; ++row)
    {
        for (int col = 0; col < m_framebufferWidth; ++col)
        {
            Vector3 pixel = Vector3(float(col) + 0.5f, float(row) + 0.5f, 1.0f);
            int triangleNum = (int)m_indices.size() / 3;
            for (int triangleIndex = 0; triangleIndex < triangleNum; ++triangleIndex)
            {
                /// 顶点坐标, 默认逆时针
                std::vector<Vector4> points;
                points.push_back(m_position[m_indices[triangleIndex * 3 + 0]]);
                points.push_back(m_position[m_indices[triangleIndex * 3 + 1]]);
                points.push_back(m_position[m_indices[triangleIndex * 3 + 2]]);
                /// 记录像素点的重心坐标
                std::vector<float> barycentricCoord;
                if (!Rasterizer::insideTriangle(pixel, points, barycentricCoord))
                {
                    continue;
                }
                /// 插值像素点的Z坐标
                /// 根据公式： 1/z = λ * 1/z0 + (1-λ) * 1/z1
                pixel.z = 1.0f / (barycentricCoord[0] / points[0].z + barycentricCoord[1] / points[1].z + barycentricCoord[2] / points[2].z);
                /// 深度测试
                if (std::fabsf(pixel.z) < m_zBuffer[row * m_framebufferWidth + col] && (-pixel.z) <= 100.0f)
                {
                    m_zBuffer[row * m_framebufferWidth + col] = std::fabsf(pixel.z);
                    // 插值像素点处的顶点属性（目前只有颜色)  使用透视矫正
                    float R = pixel.z * (barycentricCoord[0] * (float)m_color[m_indices[triangleIndex * 3]][0] / points[0].z + barycentricCoord[1] * (float)m_color[m_indices[triangleIndex * 3 + 1]][0] / points[1].z + barycentricCoord[2] * (float)m_color[m_indices[triangleIndex * 3 + 2]][0] / points[2].z);
                    float G = pixel.z * (barycentricCoord[0] * (float)m_color[m_indices[triangleIndex * 3]][1] / points[0].z + barycentricCoord[1] * (float)m_color[m_indices[triangleIndex * 3 + 1]][1] / points[1].z + barycentricCoord[2] * (float)m_color[m_indices[triangleIndex * 3 + 2]][1] / points[2].z);
                    float B = pixel.z * (barycentricCoord[0] * (float)m_color[m_indices[triangleIndex * 3]][2] / points[0].z + barycentricCoord[1] * (float)m_color[m_indices[triangleIndex * 3 + 1]][2] / points[1].z + barycentricCoord[2] * (float)m_color[m_indices[triangleIndex * 3 + 2]][2] / points[2].z);
                    /// 更新frameBuffer中颜色值
                    m_framebuffer[row * m_framebufferWidth + col] = { R / 255.0f, G / 255.0f, B / 255.0f, 1.0f };
                }
            }
        }
    }
}

void Rasterizer::clear(Buffers buffers)
{
    if ((buffers & Rasterizer::Buffers::Color) == Rasterizer::Buffers::Color)
    {
        std::fill(m_framebuffer.begin(), m_framebuffer.end(), std::vector<float>(4, 0.0f));
    }
    if ((buffers & Rasterizer::Buffers::Depth) == Rasterizer::Buffers::Depth)
    {
        std::fill(m_zBuffer.begin(), m_zBuffer.end(), std::numeric_limits<float>::infinity());
    }
}

void Rasterizer::saveResult()
{
    std::ofstream ofs;
    ofs.open(m_filePath.data());
    ofs << "P3\n"
        << m_framebufferWidth << ' ' << m_framebufferHeight << "\n255\n";
    for (int row = 0; row < m_framebufferHeight; ++row)
    {
        for (int col = 0; col < m_framebufferWidth; ++col)
        {
            float r = m_framebuffer[row * m_framebufferWidth + col][0];
            float g = m_framebuffer[row * m_framebufferWidth + col][1];
            float b = m_framebuffer[row * m_framebufferWidth + col][2];
            ofs << int(255.0f * r) << ' ' << int(255.0f * g) << ' ' << int(255.0f * b) << '\n';
        }
    }
    ofs.close();
}

} // namespace rasterizer
