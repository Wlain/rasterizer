//
// Created by william on 2021/5/3.
//

#ifndef RASTERIZER_MATRIX4_H
#define RASTERIZER_MATRIX4_H

#include "vector2.h"
#include "vector3.h"

namespace rasterizer
{
/// define a 4 x 4 floating point matrix.
class Matrix4
{
public:
    /**
     * 1  0  0  0
     * 0  1  0  0
     * 0  0  1  0
     * 0  0  0  1
     */
    Matrix4();
    Matrix4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    explicit Matrix4(const float* copy);
    explicit Matrix4(const Matrix4& copy);
    ~Matrix4();
    static const Matrix4& identity();
    static const Matrix4& zero();
    static void createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix4* dst);
    static void createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                             float targetPositionX, float targetPositionY, float targetPositionZ,
                             float upX, float upY, float upZ, Matrix4* dst);
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Matrix4* dst);
    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix4* dst);
    static void createOrthographicOffCenter(float left, float right, float bottom, float top,
                                            float zNearPlane, float zFarPlane, Matrix4* dst);
    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                                const Vector3& cameraUpPosition, Matrix4* dst);
    static void createScale(const Vector3& scale, Matrix4* dst);
    static void createScale(float xScale, float yScale, Matrix4* dst);
    static void createRotation(const Vector3& axis, float angle, Matrix4* dst);
    static void createRotationX(float angle, Matrix4* dst);
    static void createRotationY(float angle, Matrix4* dst);
    static void createRotationZ(float angle, Matrix4* dst);
    static void createTranslation(const Vector3& translation, Matrix4* dst);
    static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix4* dst);
    void add(float scalar);
    void add(float scalar, Matrix4* dst);
    void add(const Matrix4& m);
    void add(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);
    /// 计算行列式
    float determinant() const;
    void getScale(Vector3* scale) const;
    void getTranslation(Vector3* translation) const;
    void getUpVector(Vector3* dst) const;
    void getDownVector(Vector3* dst) const;
    void getLeftVector(Vector3* dst) const;
    void getRightVector(Vector3* dst) const;
    void getForwardVector(Vector3* dst) const;
    void getBaskVector(Vector3* dst) const;
    bool invert();
    bool invert(Matrix4* dst) const;
    bool isIdentity() const;
    void multiply(float scalar, Matrix4* dst) const;
    static void multiply(const Matrix4& m, float scalar, Matrix4* dst);
    void multiply(const Matrix4& m);
    static void multiply(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);
    void negate();
    void negate(Matrix4* dst) const;
    void rotate(const Vector3& axis, float angle);
    void rotate(const Vector3& axis, float angle, Matrix4* dst) const;
    void rotateX(float angle);
    void rotateX(float angle, Matrix4* dst);
    void rotateY(float angle);
    void rotateY(float angle, Matrix4* dst);
    void rotateZ(float angle);
    void rotateZ(float angle, Matrix4* dst);
    void scale(float value);
    void scale(float value, Matrix4* dst) const;
    void scale(float xScale, float yScale, float zScale);
    void scale(float xScale, float yScale, float zScale, Matrix4* dst) const;
    void scale(const Vector3& scale);
    void scale(const Vector3& scale, Matrix4* dst) const;
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    void set(const float* array);
    void set(const Matrix4& m);
    void setIdentity();
    void setZero();
    void subtract(const Matrix4& m);
    static void subtract(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);
    void transformPoint(Vector3* point) const;
    void transformPoint(const Vector3& point, Vector3* dst) const;
    void transformVector(Vector3* vector) const;
    void transformVector(const Vector3& vector, Vector3* dst) const;
    void transformVector(float x, float y, float z, float w, Vector3* dst) const;
    void translate(float x, float y, float z);
    void translate(float x, float y, float z, Matrix4* dst) const;
    void translate(const Vector3& t);
    void translate(const Vector3& t, Matrix4* dst) const;
    void transpose();
    void transpose(Matrix4* dst) const;
    inline const Matrix4 operator+(const Matrix4& m) const;
    inline Matrix4& operator+=(const Matrix4& m);
    inline const Matrix4 operator-(const Matrix4& m) const;
    inline Matrix4& operator-=(const Matrix4& m);
    inline const Matrix4 operator-() const;
    inline const Matrix4 operator*(const Matrix4& m) const;
    inline Matrix4& operator*=(const Matrix4& m);

private:
    static void createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
                                      const Vector3& cameraUpVector, const Vector3* cameraForwardVector,
                                      Matrix4* dst);

private:
    /** 内存上的排布
     * 0   4   8   12
     * 1   5   9   13
     * 2   6   10  14
     * 3   7   11  15
     */
    float m[16];
};

inline Vector3& operator*=(Vector3& v, const Matrix4& m);
inline const Vector3 operator*(const Matrix4& m, const Vector3& v);

} // namespace rasterizer

#include "matrix.inl"
#endif //RASTERIZER_MATRIX4_H
