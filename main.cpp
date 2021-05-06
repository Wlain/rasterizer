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
    triangle.initWithFrameBufferSize(RENDER_WIDTH, RENDER_HEIGHT);
    auto& camera = triangle.refCamera();
    camera.setModelMatrix(modelMatrix);
    camera.setViewMatrix(viewMatrix);
    camera.setProjectionMatrix(projectionMatrix);
    constexpr const int vertexSize = 6;
    /// 原始三角形顶点坐标,物体坐标系
    auto& vertexes = triangle.refPosition();
    vertexes.reserve(vertexSize);
    vertexes.emplace_back(-2.0f, -1.0f, 0.0f);
    vertexes.emplace_back(2.0f, -1.0f, 1.0f );
    vertexes.emplace_back(-1.0f, 1.0f, -3.0f);
    vertexes.emplace_back(-2.0f, -1.0f, 1.0f);
    vertexes.emplace_back(2.0f, -1.0f, 0.0f );
    vertexes.emplace_back(1.0f, 1.0f, -3.0f );
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
     * 视口变换的详细步骤：
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
    auto& indices = triangle.refIndices();
    indices.reserve(vertexSize);
    for (int i = 0; i < vertexSize; ++i)
    {
        indices.push_back(i);
    }
    /// 顶点颜色数组
    auto& color = triangle.refColor();
    color.push_back({255, 0, 0});
    color.push_back({0, 255, 0 });
    color.push_back({0, 0, 255 });
    color.push_back({255, 255, 0 });
    color.push_back({0, 255, 255 });
    color.push_back({255, 0, 255 });
    /// 清屏
    triangle.clear();
    /// 写入framebuffer
    triangle.draw();
    /// 设置保存的路径
    triangle.setFilePath("/Users/william/git/demo/rasterizer/output.ppm");
    /// 将framebuffer中的数据写入图片中
    triangle.saveResult();
    return 0;
}
