#include "matrix4.h"
#include "rasterizer.h"

using namespace rasterizer;
#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600

int main()
{
    /// 创建模型
    Matrix4 modelMatrix;
    Matrix4 viewMatrix{};
    Matrix4::createLookAt({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, &viewMatrix);
    Matrix4 projectionMatrix{};
    Matrix4::createPerspective(45.0f, (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1, 100.0f, &projectionMatrix);
    /// 创建光栅化器
    Rasterizer rasterizer;
    rasterizer.initWithFrameBufferSize(RENDER_WIDTH, RENDER_HEIGHT);
    auto& camera = rasterizer.refCamera();
    camera.setModelMatrix(modelMatrix);
    camera.setViewMatrix(viewMatrix);
    camera.setProjectionMatrix(projectionMatrix);
    constexpr const int vertexSize = 6;
    /// 数据准备
    /// 原始三角形顶点坐标,物体坐标系
    auto& vertexes = rasterizer.refPosition();
    vertexes.reserve(vertexSize);
    vertexes.emplace_back(2.0f, 0.0f, -2.0f, 1.0f);
    vertexes.emplace_back(0.0f, 2.0f, -2.0f, 1.0f);
    vertexes.emplace_back(-2.0f, 0.0f, -2.0f, 1.0f);
    vertexes.emplace_back(4.0f, -1.0f, -5.0f, 1.0f);
    vertexes.emplace_back(2.0f, 2.0f, -5.0f, 1.0f);
    vertexes.emplace_back(0.0f, 0.0f, -5.0f, 1.0f);
    /// 索引数组
    auto& indices = rasterizer.refIndices();
    indices.reserve(vertexSize);
    for (int i = 0; i < vertexSize; ++i)
    {
        indices.push_back(i);
    }
    /// 顶点颜色数组
    auto& color = rasterizer.refColor();
    color.push_back({ 255, 0, 0 });
    color.push_back({ 0, 255, 0 });
    color.push_back({ 0, 0, 255 });
    color.push_back({ 255, 255, 0 });
    color.push_back({ 0, 255, 255 });
    color.push_back({ 255, 0, 255 });
    /// 几何变换
    rasterizer.setGeometricTransform();
    /// 清屏
    rasterizer.clear(Rasterizer::Buffers::Depth & Rasterizer::Buffers::Color);
    /// 写入framebuffer
    rasterizer.drawTriangles();
    /// 设置保存的路径
    rasterizer.setFilePath("/Users/william/git/demo/rasterizer/output.ppm");
    /// 将framebuffer中的数据写入图片中
    rasterizer.saveResult();
    return 0;
}
