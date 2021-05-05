//
// Created by william on 2021/5/4.
//

#ifndef RASTERIZER_UTILFUNCTION_H
#define RASTERIZER_UTILFUNCTION_H

#include "commonDefine.h"
#include "math/vector2.h"

using namespace rasterizer;

bool edgeFunction(const Vector2& point, const Vector2& aVertex, const Vector2& bVertex, const Vector2& cVertex, std::vector<float>& texCoord);



#endif //RASTERIZER_UTILFUNCTION_H
