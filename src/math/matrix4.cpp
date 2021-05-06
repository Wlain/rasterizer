//
// Created by william on 2021/5/3.
//

#include "matrix4.h"

#include "commonDefine.h"
#include "logger.h"
#include "vector3.h"

namespace rasterizer
{
Matrix4::Matrix4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Matrix4::Matrix4()
{
    *this = Matrix4::identity();
}

Matrix4::Matrix4(const float* copy)
{
}

Matrix4::Matrix4(const Matrix4& copy)
{
}

Matrix4::~Matrix4() = default;

const Matrix4& Matrix4::identity()
{
    static Matrix4 m(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    return m;
}
void Matrix4::createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix4* dst)
{
    createLookAt(eyePosition.x, eyePosition.y, eyePosition.z, targetPosition.x, targetPosition.y, targetPosition.z,
                 up.x, up.y, up.z, dst);
}

void Matrix4::createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ, float targetPositionX, float targetPositionY, float targetPositionZ, float upX, float upY, float upZ, Matrix4* dst)
{
    RAS_ASSERT(dst);
    Vector3 eye(eyePositionX, eyePositionY, eyePositionZ);
    Vector3 target(targetPositionX, targetPositionY, targetPositionZ);
    Vector3 up(upX, upY, upZ);
    up.normalize();

    Vector3 zaxis;
    Vector3::subtract(eye, target, &zaxis);
    zaxis.normalize();

    Vector3 xaxis;
    Vector3::cross(up, zaxis, &xaxis);
    xaxis.normalize();

    Vector3 yaxis;
    Vector3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();

    dst->m[0] = xaxis.x;
    dst->m[1] = yaxis.x;
    dst->m[2] = zaxis.x;
    dst->m[3] = 0.0f;

    dst->m[4] = xaxis.y;
    dst->m[5] = yaxis.y;
    dst->m[6] = zaxis.y;
    dst->m[7] = 0.0f;

    dst->m[8] = xaxis.z;
    dst->m[9] = yaxis.z;
    dst->m[10] = zaxis.z;
    dst->m[11] = 0.0f;

    dst->m[12] = -Vector3::dot(xaxis, eye);
    dst->m[13] = -Vector3::dot(yaxis, eye);
    dst->m[14] = -Vector3::dot(zaxis, eye);
    dst->m[15] = 1.0f;
}

void Matrix4::createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Matrix4* dst)
{
    RAS_ASSERT(dst);
    RAS_ASSERT(zFarPlane != zNearPlane);
    float f_n = 1.0f / (zFarPlane - zNearPlane);
    float theta = MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
    if (fabs(fmod(theta, MATH_PIOVER2)) < MATH_EPSILON)
    {
        RAS_ERROR("Invalid field of view value (%d) causes attempted calculation tan(%d), which is undefined.", fieldOfView, theta);
        return;
    }
    float divisor = tan(theta);
    RAS_ASSERT(divisor);
    float factor = 1.0f / divisor;

    memset((void*)dst, 0, sizeof(float) * 16);

    RAS_ASSERT(aspectRatio);
    dst->m[0] = (1.0f / aspectRatio) * factor;
    dst->m[5] = factor;
    dst->m[10] = (-(zFarPlane + zNearPlane)) * f_n;
    dst->m[11] = -1.0f;
    dst->m[14] = -2.0f * zFarPlane * zNearPlane * f_n;
}

void Matrix4::transformPoint(Vector3* point) const
{
    RAS_ASSERT(point);
    transformVector(point->x, point->y, point->z, 1.0f, point);
}

void Matrix4::transformVector(float x, float y, float z, float w, Vector3* dst) const
{
    RAS_ASSERT(dst);
    dst->x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst->y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst->z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
}

void Matrix4::set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0] = m11;
    m[1] = m21;
    m[2] = m31;
    m[3] = m41;
    m[4] = m12;
    m[5] = m22;
    m[6] = m32;
    m[7] = m42;
    m[8] = m13;
    m[9] = m23;
    m[10] = m33;
    m[11] = m43;
    m[12] = m14;
    m[13] = m24;
    m[14] = m34;
    m[15] = m44;
}

void Matrix4::createOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane, Matrix4* dst)
{
}

void Matrix4::transformVector(Vector4* vector) const
{
    RAS_ASSERT(vector);
    transformVector(*vector, vector);
}

void Matrix4::transformVector(const Vector4& vector, Vector4* dst) const
{
    RAS_ASSERT(dst);
    dst->x = vector.x * m[0] + vector.y * m[4] + vector.z * m[8] + vector.w * m[12];
    dst->y = vector.x * m[1] + vector.y * m[5] + vector.z * m[9] + vector.w * m[13];
    dst->z = vector.x * m[2] + vector.y * m[6] + vector.z * m[10] + vector.w * m[14];
    dst->w = vector.x * m[3] + vector.y * m[7] + vector.z * m[11] + vector.w * m[15];
}
} // namespace rasterizer