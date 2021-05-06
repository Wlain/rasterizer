//
// Created by william on 2021/5/4.
//

#include "utilFunction.h"
#include "vector2.h"

using namespace rasterizer;

bool insideTriangle(const Vector3& point, const std::vector<Vector4>& vertexes, std::vector<float>& barycentricCoord)
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