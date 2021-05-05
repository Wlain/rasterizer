//
// Created by william on 2021/5/3.
//

#ifndef RASTERIZER_COMMONDEFINE_H
#define RASTERIZER_COMMONDEFINE_H

/// C/C++
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>
#include <array>

/// common functions from C into global namespace
using std::atan2f;
using std::fabs;
using std::fabsf;

// Math
#define MATH_DEG_TO_RAD(x) ((x)*0.0174532925f)
#define MATH_PIOVER2 1.57079632679489661923f
#define MATH_EPSILON 0.000001f

/// assert macros
#ifdef _DEBUG
    #define RAS_ASSERT(expression) assert(expression)
#else
    #define RAS_ASSERT(expression)
#endif

/// Error macro
#ifdef RAS_ERRORS_AS_WARNINGS
    #define RAS_ERROR RAS_WARN
#else
    #define RAS_ERROR(...)                                  \
        do                                                  \
        {                                                   \
            Logger::log(Logger::Level::Error, __VA_ARGS__); \
            RAS_ASSERT(0);                                  \
        } while (0)
#endif

/// Info macro
#define RAS_INFO(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Info, __VA_ARGS__); \
    } while (0)

/// Warn macro
#define RAS_WARN(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Warn, __VA_ARGS__); \
    } while (0)

#endif //RASTERIZER_COMMONDEFINE_H
