//
// Created by william on 2021/5/3.
//

#ifndef RASTERIZER_COMMONDEFINE_H
#define RASTERIZER_COMMONDEFINE_H

/// C/C++
#include <algorithm>
#include <cmath>
#include <functional>

/// Bring common functions from C into global namespace
using std::atan2f;
using std::fabs;
using std::fabsf;

/// assert macros
#ifdef _DEBUG
    #define RAS_ASSERT(expression) assert(expression)
#else
    #define RAS_ASSERT(expression)
#endif

/// Math
#define MATH_FLOAT_SMALL 1.0e-37f
#define MATH_TOLERANCE 2e-37f

#endif //RASTERIZER_COMMONDEFINE_H
