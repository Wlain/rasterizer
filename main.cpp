#include "camera.h"
#include "commonDefine.h"
#include "logger.h"
#include "matrix4.h"
#include "rasterizer.h"
#include "vector2.h"

using namespace rasterizer;
#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600

int main()
{
    /// 创建模型
    Matrix4 modelMatrix;
    Matrix4 viewMatrix{};
    Matrix4::createLookAt({ 0.0f, 0.0f, 3.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, &viewMatrix);
    Matrix4 projectionMatrix{};
    Matrix4::createPerspective(60.0f, (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1f, 100.0f, &projectionMatrix);
    /// 创建图元
    Rasterizer triangle;
    auto& camera = triangle.refCamera();
    camera.setModelMatrix(modelMatrix);
    camera.setViewMatrix(viewMatrix);
    camera.setProjectionMatrix(projectionMatrix);
    constexpr const int vertexSize = 6;
    /// 原始三角形顶点坐标,物体坐标系
    Vector3 vertexes[vertexSize];
    vertexes[0] = { -2.0f, -1.0f, 0.0f };
    vertexes[1] = { 2.0f, -1.0f, 1.0f };
    vertexes[2] = { -1.0f, 1.0f, -3.0f };
    vertexes[3] = { -2.0f, -1.0f, 1.0f };
    vertexes[4] = { 2.0f, -1.0f, 0.0f };
    vertexes[5] = { 1.0f, 1.0f, -3.0f };
    /// 模型变换 -> 世界坐标系
    for (auto& v : vertexes)
    {
        modelMatrix.transformPoint(&v);
    }
    /// 视图变换 -> 观察者坐标系
    for (auto& v : vertexes)
    {
        viewMatrix.transformPoint(&v);
    }
    /// 投影变换 -> 裁剪坐标系
    for (auto& v : vertexes)
    {
        projectionMatrix.transformPoint(&v);
    }
    /// 透视除法 -> 规范化坐标系
    const auto w = 1.0f;
    for (auto& v : vertexes)
    {
        v.x / w;
        v.y / w;
        v.z / w;
    }
    /// 视口变换 -> 屏幕坐标系
    /**
     * 1.丢弃z值
     * 2.缩放
     * 3.平移
     * */
    float halfScreenWidth = RENDER_WIDTH / 2.0f;
    float halfScreenHeight = RENDER_HEIGHT / 2.0f;
    for (auto& v : vertexes)
    {
        v.x = (v.x + 1.0f) + halfScreenWidth;
        v.y = (1.0f - v.y) + halfScreenHeight;
    }

    /// 索引数组
    std::vector<unsigned short> indices;
    indices.reserve(5);
    for (int i = 0; i < 5; ++i)
    {
        indices.push_back(i);
    }
    /// 顶点颜色数组
    std::array<std::array<unsigned char, 3>, 6> colors{};
    colors[0] = { 255, 0, 0 };
    colors[1] = { 0, 255, 0 };
    colors[2] = { 0, 0, 255 };
    colors[3] = { 255, 255, 0 };
    colors[4] = { 0, 255, 255 };
    colors[5] = { 255, 0, 255 };

    /// 光栅化步骤：
    /**
     * 1.判断每个像素点是否在屏幕对应的三角形中。
     * 2.如果在三角形中，计算其三角形重心坐标，并进行顶点属性插值
     * 3.将颜色信息写入frameBuffer
     * */
    size_t bufferSize = RENDER_WIDTH * RENDER_HEIGHT;
    auto& zBuffer = triangle.refBufferZ();
    zBuffer.resize(bufferSize);
    auto& framebuffer = triangle.refFramebuffer();
    framebuffer.resize(bufferSize);
    /// 初始化Buffer，FrameBuffer
    for (int i = 0; i < bufferSize; ++i)
    {
        zBuffer[i] = FLT_MAX;
        framebuffer[i] = std::vector<float>(4, 0.0f);
    }
    /// 开始遍历像素
    bool isOpenMsaa = false;
    for (int row = 0; row < RENDER_WIDTH; ++row)
    {
        for (int col = 0; col < RENDER_HEIGHT; ++col)
        {
            Vector3 pixel = Vector3(float(col) + 0.5f, float(row) + 0.5f, 1.0f);
            int triangleNum = (int)indices.size() / 3;
            for (int triangleIndex = 0; triangleIndex < triangleNum; ++triangleIndex)
            {
                /// 顶点坐标, 默认逆时针
                std::vector<Vector3> points;
                points.push_back(vertexes[indices[triangleIndex * 3]]);
                points.push_back(vertexes[indices[triangleIndex * 1]]);
                points.push_back(vertexes[indices[triangleIndex * 2]]);

                /// 顶点颜色
                std::array<unsigned char, 3> pointColorA = colors[indices[triangleIndex * 3]];
                std::array<unsigned char, 3> pointColorB = colors[indices[triangleIndex * 3 + 1]];
                std::array<unsigned char, 3> pointColorC = colors[indices[triangleIndex * 3 + 2]];

                /// 记录像素点的重心坐标
                std::vector<float> barycentricCoord;

                ///edge Function
                if (!Rasterizer::insideTriangle(pixel, points, barycentricCoord))
                {
                    continue;
                }
                else
                {
                    /// 插值像素点的Z坐标
                    // 1 / z = λ * 1/ z0 + (1-λ) * 1 /z1
                    pixel.z = 1.0f / (barycentricCoord[0] / points[0].z + barycentricCoord[1] / points[1].z + barycentricCoord[2] / points[2].z);

                    /// 深度测试 只有Z坐标小于0的物体才能被看到
                    if (pixel.z < 1e-5 && std::fabsf(pixel.z) < zBuffer[row * RENDER_WIDTH + col] && (-pixel.z) >= 0.1f && (-pixel.z) <= 100.0f)
                    {
                        zBuffer[row * RENDER_WIDTH + col] = std::fabsf(pixel.z);
                        // 插值像素点处的顶点属性（目前只有颜色)  使用透视矫正
                        float R = pixel.z * (barycentricCoord[0] * float(pointColorA[0]) / points[0].z + barycentricCoord[1] * float(pointColorB[0]) / points[1].z + barycentricCoord[2] * float(pointColorC[0]) / points[2].z);
                        float G = pixel.z * (barycentricCoord[0] * float(pointColorA[1]) / points[0].z + barycentricCoord[1] * float(pointColorB[1]) / points[1].z + barycentricCoord[2] * float(pointColorC[1]) / points[2].z);
                        float B = pixel.z * (barycentricCoord[0] * float(pointColorA[2]) / points[0].z + barycentricCoord[1] * float(pointColorB[2]) / points[1].z + barycentricCoord[2] * float(pointColorC[2]) / points[2].z);

                        /// 更新frameBuffer中颜色值
                        std::vector<float> piexlColor = std::vector<float>(4);
                        piexlColor[0] = R / 255.0f;
                        piexlColor[1] = G / 255.0f;
                        piexlColor[2] = B / 255.0f;
                        piexlColor[3] = 1.0f;
                        framebuffer[row * RENDER_WIDTH + col] = piexlColor;
                    }
                }
            }
        }
    }
    //将framebuffer中的数据写入图片中
    std::ofstream ofs;
    ofs.open("/Users/william/git/demo/rasterizer/output.ppm");
    ofs << "P3\n"
        << RENDER_WIDTH << ' ' << RENDER_HEIGHT << "\n255\n";
    for (int row = 0; row < RENDER_HEIGHT; ++row)
    {
        for (int col = 0; col < RENDER_WIDTH; ++col)
        {
            float r = framebuffer[row * RENDER_WIDTH + col][0];
            float g = framebuffer[row * RENDER_WIDTH + col][1];
            float b = framebuffer[row * RENDER_WIDTH + col][2];
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            ofs << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    ofs.close();
    return 0;
}
