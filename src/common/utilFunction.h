//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_UTILFUNCTION_H
#define RASTERIZER_UTILFUNCTION_H
#include "commonDefine.h"
#include "vector3.h"
#include "vector4.h"

#define MATRIX_SIZE (sizeof(float) * 16)

/**
   * 判断当前点是否在三角形内部
   * @param point 当前点坐标
   * @param vertexes 三角形顶点坐标
   * @param barycentricCoord 重心坐标
   * @return
   */
bool insideTriangle(const rasterizer::Vector3& point, const std::vector<rasterizer::Vector4>& vertexes, std::vector<float>& barycentricCoord);

#endif //RASTERIZER_UTILFUNCTION_H
