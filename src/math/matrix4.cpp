//
// Created by william on 2021/5/3.
//

#include "matrix4.h"

#include "commonDefine.h"
#include "logger.h"
#include "utilFunction.h"
#include "vector3.h"

namespace rasterizer
{
static const float MATRIX_IDENTITY[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

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
    set(m);
}

Matrix4::Matrix4(const Matrix4& copy)
{
    memcpy(m, copy.m, MATRIX_SIZE);
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

const Matrix4& Matrix4::zero()
{
    static Matrix4 m(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);
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

void Matrix4::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix4* dst)
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
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

    memset((void*)dst, 0, MATRIX_SIZE);

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
    RAS_ASSERT(dst);
    RAS_ASSERT(right != left);
    RAS_ASSERT(top != bottom);
    RAS_ASSERT(zFarPlane != zNearPlane);
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

void Matrix4::set(const float* array)
{
    RAS_ASSERT(array);
    memcpy(m, array, MATRIX_SIZE);
}

void Matrix4::set(const Matrix4& m)
{
    memcpy(this->m, m.m, MATRIX_SIZE);
}

void Matrix4::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector, Matrix4* dst)
{
    createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, nullptr, dst);
}

void Matrix4::createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector, const Vector3* cameraForwardVector, Matrix4* dst)
{
    Vector3 delta(objectPosition, cameraPosition);
    bool isSufficientDelta = delta.lengthSquared() > MATH_EPSILON;
    dst->setIdentity();
    dst->m[3] = objectPosition.x;
    dst->m[7] = objectPosition.y;
    dst->m[11] = objectPosition.z;
    if (cameraForwardVector || isSufficientDelta)
    {
        Vector3 target = isSufficientDelta ? cameraPosition : (objectPosition - *cameraForwardVector);
        // A billboard is the inverse of a lookAt rotation
        Matrix4 lookAt;
        createLookAt(objectPosition, target, cameraUpVector, &lookAt);
        dst->m[0] = lookAt.m[0];
        dst->m[1] = lookAt.m[4];
        dst->m[2] = lookAt.m[8];
        dst->m[4] = lookAt.m[1];
        dst->m[5] = lookAt.m[5];
        dst->m[6] = lookAt.m[9];
        dst->m[8] = lookAt.m[2];
        dst->m[9] = lookAt.m[6];
        dst->m[10] = lookAt.m[10];
    }
}
void Matrix4::createScale(const Vector3& scale, Matrix4* dst)
{
    createScale(scale.x, scale.y, scale.z, dst);
}

void Matrix4::createScale(float xScale, float yScale, float zScale, Matrix4* dst)
{
    RAS_ASSERT(dst);
    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);
    dst->m[0] = xScale;
    dst->m[5] = yScale;
    dst->m[10] = zScale;
}
void Matrix4::createRotation(const Vector3& axis, float angle, Matrix4* dst)
{
    RAS_ASSERT(dst);
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float n = x * x + y * y + z * z;
    if (n != 1.0f)
    {
        // not normalized.
        n = sqrt(n);
        // prevent divide too close to zero.
        if (n > 0.000001f)
        {
            n = 1.0f / n;
            x *= n;
            y *= n;
            z *= n;
        }
    }
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float txy = tx * y;
    float txz = tx * z;
    float tyz = ty * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;

    dst->m[0] = c + tx * x;
    dst->m[1] = txy + sz;
    dst->m[2] = txz - sy;
    dst->m[3] = 0.0f;

    dst->m[4] = txy - sz;
    dst->m[5] = c + ty * y;
    dst->m[6] = tyz + sx;
    dst->m[7] = 0.0f;

    dst->m[8] = txz + sy;
    dst->m[9] = tyz - sx;
    dst->m[10] = c + tz * z;
    dst->m[11] = 0.0f;

    dst->m[12] = 0.0f;
    dst->m[13] = 0.0f;
    dst->m[14] = 0.0f;
    dst->m[15] = 1.0f;
}

void Matrix4::createRotationX(float angle, Matrix4* dst)
{
    RAS_ASSERT(dst);
    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);
    float c = cos(angle);
    float s = sin(angle);

    dst->m[5] = c;
    dst->m[6] = s;
    dst->m[9] = -s;
    dst->m[10] = c;
}

void Matrix4::createRotationY(float angle, Matrix4* dst)
{
    RAS_ASSERT(dst);

    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0] = c;
    dst->m[2] = -s;
    dst->m[8] = s;
    dst->m[10] = c;
}

void Matrix4::createRotationZ(float angle, Matrix4* dst)
{
    RAS_ASSERT(dst);

    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0] = c;
    dst->m[1] = s;
    dst->m[4] = -s;
    dst->m[5] = c;
}

void Matrix4::createTranslation(const Vector3& translation, Matrix4* dst)
{
    createTranslation(translation.x, translation.y, translation.z, dst);
}

void Matrix4::createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix4* dst)
{
    RAS_ASSERT(dst);

    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[12] = xTranslation;
    dst->m[13] = yTranslation;
    dst->m[14] = zTranslation;
}

void Matrix4::add(float scalar)
{
    add(scalar, this);
}

void Matrix4::add(float scalar, Matrix4* dst)
{
    RAS_ASSERT(dst);
    dst->m[0] = m[0] + scalar;
    dst->m[1] = m[1] + scalar;
    dst->m[2] = m[2] + scalar;
    dst->m[3] = m[3] + scalar;
    dst->m[4] = m[4] + scalar;
    dst->m[5] = m[5] + scalar;
    dst->m[6] = m[6] + scalar;
    dst->m[7] = m[7] + scalar;
    dst->m[8] = m[8] + scalar;
    dst->m[9] = m[9] + scalar;
    dst->m[10] = m[10] + scalar;
    dst->m[11] = m[11] + scalar;
    dst->m[12] = m[12] + scalar;
    dst->m[13] = m[13] + scalar;
    dst->m[14] = m[14] + scalar;
    dst->m[15] = m[15] + scalar;
}
void Matrix4::add(const Matrix4& m)
{
    add(*this, m, this);
}

void Matrix4::add(const Matrix4& m1, const Matrix4& m2, Matrix4* dst)
{
    RAS_ASSERT(dst);
    dst->m[0]  = m1.m[0]  + m2.m[0];
    dst->m[1]  = m1.m[1]  + m2.m[1];
    dst->m[2]  = m1.m[2]  + m2.m[2];
    dst->m[3]  = m1.m[3]  + m2.m[3];
    dst->m[4]  = m1.m[4]  + m2.m[4];
    dst->m[5]  = m1.m[5]  + m2.m[5];
    dst->m[6]  = m1.m[6]  + m2.m[6];
    dst->m[7]  = m1.m[7]  + m2.m[7];
    dst->m[8]  = m1.m[8]  + m2.m[8];
    dst->m[9]  = m1.m[9]  + m2.m[9];
    dst->m[10] = m1.m[10] + m2.m[10];
    dst->m[11] = m1.m[11] + m2.m[11];
    dst->m[12] = m1.m[12] + m2.m[12];
    dst->m[13] = m1.m[13] + m2.m[13];
    dst->m[14] = m1.m[14] + m2.m[14];
    dst->m[15] = m1.m[15] + m2.m[15];
}
float Matrix4::determinant() const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    // calculate the determinant.
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}
void Matrix4::getScale(Vector3* scale) const
{
}
void Matrix4::getTranslation(Vector3* translation) const
{
}
void Matrix4::getUpVector(Vector3* dst) const
{
}
void Matrix4::getDownVector(Vector3* dst) const
{
}
void Matrix4::getLeftVector(Vector3* dst) const
{
}
void Matrix4::getRightVector(Vector3* dst) const
{
}
void Matrix4::getForwardVector(Vector3* dst) const
{
}
void Matrix4::getBaskVector(Vector3* dst) const
{
}
bool Matrix4::invert()
{
    return false;
}
bool Matrix4::invert(Matrix4* dst) const
{
    return false;
}
bool Matrix4::isIdentity() const
{
    return (memcmp(m, MATRIX_IDENTITY, MATRIX_SIZE) == 0);
}
void Matrix4::multiply(float scalar, Matrix4* dst) const
{
    multiply(*this, scalar, dst);
}
void Matrix4::multiply(const Matrix4& m, float scalar, Matrix4* dst)
{
}
void Matrix4::multiply(const Matrix4& m)
{
}
void Matrix4::multiply(const Matrix4& m1, const Matrix4& m2, Matrix4* dst)
{
}
void Matrix4::negate()
{
}
void Matrix4::negate(Matrix4* dst) const
{
}
void Matrix4::rotate(const Vector3& axis, float angle)
{
}
void Matrix4::rotate(const Vector3& axis, float angle, Matrix4* dst) const
{
}
void Matrix4::rotateX(float angle)
{
}
void Matrix4::rotateX(float angle, Matrix4* dst)
{
}
void Matrix4::rotateY(float angle)
{
}
void Matrix4::rotateY(float angle, Matrix4* dst)
{
}

} // namespace rasterizer