//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_EZGLMATRIX4_H
#define EZGL_EZGLMATRIX4_H

#include "EZGLBasicTypes.h"
#include "EZGLMatrixTypes.h"
#include "EZGLVectorTypes.h"

#include <math.h>

const ELMatrix4 ELMatrix4Identity = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
};


/*
 m30, m31, and m32 correspond to the translation values tx, ty, tz, respectively.
 */
EL_INLINE ELMatrix4 ELMatrix4Make(ELFloat m00, ELFloat m01, ELFloat m02, ELFloat m03,
                                     ELFloat m10, ELFloat m11, ELFloat m12, ELFloat m13,
                                     ELFloat m20, ELFloat m21, ELFloat m22, ELFloat m23,
                                     ELFloat m30, ELFloat m31, ELFloat m32, ELFloat m33);

/*
 m03, m13, and m23 correspond to the translation values tx, ty, tz, respectively.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeAndTranspose(ELFloat m00, ELFloat m01, ELFloat m02, ELFloat m03,
                                                 ELFloat m10, ELFloat m11, ELFloat m12, ELFloat m13,
                                                 ELFloat m20, ELFloat m21, ELFloat m22, ELFloat m23,
                                                 ELFloat m30, ELFloat m31, ELFloat m32, ELFloat m33);

/*
 m[12], m[13], and m[14] correspond to the translation values tx, ty, and tz, respectively.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeWithArray(ELFloat values[16]);

/*
 m[3], m[7], and m[11] correspond to the translation values tx, ty, and tz, respectively.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeWithArrayAndTranspose(ELFloat values[16]);

/*
 row0, row1, and row2's last component should correspond to the translation values tx, ty, and tz, respectively.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeWithRows(ELVector4 row0,
ELVector4 row1,
        ELVector4 row2,
ELVector4 row3);

/*
 column3's first three component should correspond to the translation values tx, ty, and tz.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeWithColumns(ELVector4 column0,
ELVector4 column1,
        ELVector4 column2,
ELVector4 column3);

/*
 The quaternion will be normalized before conversion.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeWithQuaternion(ELQuaternion quaternion);

EL_INLINE ELMatrix4 ELMatrix4MakeTranslation(ELFloat tx, ELFloat ty, ELFloat tz);
EL_INLINE ELMatrix4 ELMatrix4MakeScale(ELFloat sx, ELFloat sy, ELFloat sz);
EL_INLINE ELMatrix4 ELMatrix4MakeRotation(ELFloat radians, ELFloat x, ELFloat y, ELFloat z);

EL_INLINE ELMatrix4 ELMatrix4MakeXRotation(ELFloat radians);
EL_INLINE ELMatrix4 ELMatrix4MakeYRotation(ELFloat radians);
EL_INLINE ELMatrix4 ELMatrix4MakeZRotation(ELFloat radians);

/*
 Equivalent to gluPerspective.
 */
EL_INLINE ELMatrix4 ELMatrix4MakePerspective(ELFloat fovyRadians, ELFloat aspect, ELFloat nearZ, ELFloat farZ);

/*
 Equivalent to glFrustum.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeFrustum(ELFloat left, ELFloat right,
                                            ELFloat bottom, ELFloat top,
                                            ELFloat nearZ, ELFloat farZ);

/*
 Equivalent to glOrtho.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeOrtho(ELFloat left, ELFloat right,
                                          ELFloat bottom, ELFloat top,
                                          ELFloat nearZ, ELFloat farZ);

/*
 Equivalent to gluLookAt.
 */
EL_INLINE ELMatrix4 ELMatrix4MakeLookAt(ELFloat eyeX, ELFloat eyeY, ELFloat eyeZ,
                                           ELFloat centerX, ELFloat centerY, ELFloat centerZ,
                                           ELFloat upX, ELFloat upY, ELFloat upZ);

/*
 Returns the upper leftVector 3x3 portion of the 4x4 matrix.
 */
EL_INLINE ELMatrix3 ELMatrix4GetMatrix3(ELMatrix4 matrix);
/*
 Returns the upper leftVector 2x2 portion of the 4x4 matrix.
 */
EL_INLINE ELMatrix2 ELMatrix4GetMatrix2(ELMatrix4 matrix);

/*
 ELMatrix4GetRow returns vectors for rows 0, 1, and 2 whose last component will be the translation value tx, ty, and tz, respectively.
 Valid row values range from 0 to 3, inclusive.
 */
EL_INLINE ELVector4 ELMatrix4GetRow(ELMatrix4 matrix, int row);
/*
 ELMatrix4GetColumn returns a vector for column 3 whose first three component will be the translation values tx, ty, and tz.
 Valid column values range from 0 to 3, inclusive.
 */
EL_INLINE ELVector4 ELMatrix4GetColumn(ELMatrix4 matrix, int column);

/*
 ELMatrix4SetRow expects that the vector for row 0, 1, and 2 will have a translation value as its last component.
 Valid row values range from 0 to 3, inclusive.
 */
EL_INLINE ELMatrix4 ELMatrix4SetRow(ELMatrix4 matrix, int row, ELVector4 vector);
/*
 ELMatrix4SetColumn expects that the vector for column 3 will contain the translation values tx, ty, and tz as its first three component, respectively.
 Valid column values range from 0 to 3, inclusive.
 */
EL_INLINE ELMatrix4 ELMatrix4SetColumn(ELMatrix4 matrix, int column, ELVector4 vector);

EL_INLINE ELMatrix4 ELMatrix4Transpose(ELMatrix4 matrix);

static ELMatrix4 ELMatrix4Invert(ELMatrix4 matrix, bool * __nullable isInvertible);
ELMatrix4 ELMatrix4InvertAndTranspose(ELMatrix4 matrix, bool * __nullable isInvertible);

EL_INLINE ELMatrix4 ELMatrix4Multiply(ELMatrix4 matrixLeft, ELMatrix4 matrixRight);

EL_INLINE ELMatrix4 ELMatrix4Add(ELMatrix4 matrixLeft, ELMatrix4 matrixRight);
EL_INLINE ELMatrix4 ELMatrix4Subtract(ELMatrix4 matrixLeft, ELMatrix4 matrixRight);

EL_INLINE ELMatrix4 ELMatrix4Translate(ELMatrix4 matrix, ELFloat tx, ELFloat ty, ELFloat tz);
EL_INLINE ELMatrix4 ELMatrix4TranslateWithVector3(ELMatrix4 matrix, ELVector3 translationVector);
/*
 The last component of the ELVector4, translationVector, is ignored.
 */
EL_INLINE ELMatrix4 ELMatrix4TranslateWithVector4(ELMatrix4 matrix, ELVector4 translationVector);

EL_INLINE ELMatrix4 ELMatrix4Scale(ELMatrix4 matrix, ELFloat sx, ELFloat sy, ELFloat sz);
EL_INLINE ELMatrix4 ELMatrix4ScaleWithVector3(ELMatrix4 matrix, ELVector3 scaleVector);
/*
 The last component of the ELVector4, scaleVector, is ignored.
 */
EL_INLINE ELMatrix4 ELMatrix4ScaleWithVector4(ELMatrix4 matrix, ELVector4 scaleVector);

EL_INLINE ELMatrix4 ELMatrix4Rotate(ELMatrix4 matrix, ELFloat radians, ELFloat x, ELFloat y, ELFloat z);
EL_INLINE ELMatrix4 ELMatrix4RotateWithVector3(ELMatrix4 matrix, ELFloat radians, ELVector3 axisVector);
/*
 The last component of the ELVector4, axisVector, is ignored.
 */
EL_INLINE ELMatrix4 ELMatrix4RotateWithVector4(ELMatrix4 matrix, ELFloat radians, ELVector4 axisVector);

EL_INLINE ELMatrix4 ELMatrix4RotateX(ELMatrix4 matrix, ELFloat radians);
EL_INLINE ELMatrix4 ELMatrix4RotateY(ELMatrix4 matrix, ELFloat radians);
EL_INLINE ELMatrix4 ELMatrix4RotateZ(ELMatrix4 matrix, ELFloat radians);

/*
 Assumes 0 in the w component.
 */
EL_INLINE ELVector3 ELMatrix4MultiplyVector3(ELMatrix4 matrixLeft, ELVector3 vectorRight);
/*
 Assumes 1 in the w component.
 */
EL_INLINE ELVector3 ELMatrix4MultiplyVector3WithTranslation(ELMatrix4 matrixLeft, ELVector3 vectorRight);
/*
 Assumes 1 in the w component and divides the resulting vector by w before returning.
 */
EL_INLINE ELVector3 ELMatrix4MultiplyAndProjectVector3(ELMatrix4 matrixLeft, ELVector3 vectorRight);

/*
 Assumes 0 in the w component.
 */
EL_INLINE void ELMatrix4MultiplyVector3Array(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount);
/*
 Assumes 1 in the w component.
 */
EL_INLINE void ELMatrix4MultiplyVector3ArrayWithTranslation(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount);
/*
 Assumes 1 in the w component and divides the resulting vector by w before returning.
 */
EL_INLINE void ELMatrix4MultiplyAndProjectVector3Array(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount);

EL_INLINE ELVector4 ELMatrix4MultiplyVector4(ELMatrix4 matrixLeft, ELVector4 vectorRight);

EL_INLINE void ELMatrix4MultiplyVector4Array(ELMatrix4 matrix, ELVector4 * vectors, ELSize vectorCount);



EL_INLINE ELMatrix4 ELMatrix4Make(ELFloat m00, ELFloat m01, ELFloat m02, ELFloat m03,
                                     ELFloat m10, ELFloat m11, ELFloat m12, ELFloat m13,
                                     ELFloat m20, ELFloat m21, ELFloat m22, ELFloat m23,
                                     ELFloat m30, ELFloat m31, ELFloat m32, ELFloat m33)
{
    ELMatrix4 m = { m00, m01, m02, m03,
                     m10, m11, m12, m13,
                     m20, m21, m22, m23,
                     m30, m31, m32, m33 };
    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeAndTranspose(ELFloat m00, ELFloat m01, ELFloat m02, ELFloat m03,
                                                 ELFloat m10, ELFloat m11, ELFloat m12, ELFloat m13,
                                                 ELFloat m20, ELFloat m21, ELFloat m22, ELFloat m23,
                                                 ELFloat m30, ELFloat m31, ELFloat m32, ELFloat m33)
{
    ELMatrix4 m = { m00, m10, m20, m30,
                     m01, m11, m21, m31,
                     m02, m12, m22, m32,
                     m03, m13, m23, m33 };
    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeWithArray(ELFloat values[16])
{
    ELMatrix4 m = { values[0], values[1], values[2], values[3],
                     values[4], values[5], values[6], values[7],
                     values[8], values[9], values[10], values[11],
                     values[12], values[13], values[14], values[15] };
    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeWithArrayAndTranspose(ELFloat values[16])
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4x4_t m = vld4q_f32(values);
    return *(ELMatrix4 *)&m;
#else
    ELMatrix4 m = { values[0], values[4], values[8], values[12],
                     values[1], values[5], values[9], values[13],
                     values[2], values[6], values[10], values[14],
                     values[3], values[7], values[11], values[15] };
    return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4MakeWithRows(ELVector4 row0,
ELVector4 row1,
        ELVector4 row2,
ELVector4 row3)
{
ELMatrix4 m = { row0.v[0], row1.v[0], row2.v[0], row3.v[0],
                 row0.v[1], row1.v[1], row2.v[1], row3.v[1],
                 row0.v[2], row1.v[2], row2.v[2], row3.v[2],
                 row0.v[3], row1.v[3], row2.v[3], row3.v[3] };
return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeWithColumns(ELVector4 column0,
ELVector4 column1,
        ELVector4 column2,
ELVector4 column3)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t m;
    m.val[0] = vld1q_f32(column0.v);
    m.val[1] = vld1q_f32(column1.v);
    m.val[2] = vld1q_f32(column2.v);
    m.val[3] = vld1q_f32(column3.v);
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    *((__m128*)&m.m[0])  = *(__m128*)&column0;
    *((__m128*)&m.m[4])  = *(__m128*)&column1;
    *((__m128*)&m.m[8])  = *(__m128*)&column2;
    *((__m128*)&m.m[12]) = *(__m128*)&column3;
    return m;
#else
ELMatrix4 m = { column0.v[0], column0.v[1], column0.v[2], column0.v[3],
                 column1.v[0], column1.v[1], column1.v[2], column1.v[3],
                 column2.v[0], column2.v[1], column2.v[2], column2.v[3],
                 column3.v[0], column3.v[1], column3.v[2], column3.v[3] };
return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4MakeWithQuaternion(ELQuaternion quaternion)
{
quaternion = ELQuaternionNormalize(quaternion);

ELFloat x = quaternion.q[0];
ELFloat y = quaternion.q[1];
ELFloat z = quaternion.q[2];
ELFloat w = quaternion.q[3];

ELFloat _2x = x + x;
ELFloat _2y = y + y;
ELFloat _2z = z + z;
ELFloat _2w = w + w;

ELMatrix4 m = { 1.0f - _2y * y - _2z * z,
                 _2x * y + _2w * z,
                 _2x * z - _2w * y,
                 0.0f,
                 _2x * y - _2w * z,
                 1.0f - _2x * x - _2z * z,
                 _2y * z + _2w * x,
                 0.0f,
                 _2x * z + _2w * y,
                 _2y * z - _2w * x,
                 1.0f - _2x * x - _2y * y,
                 0.0f,
                 0.0f,
                 0.0f,
                 0.0f,
                 1.0f };

return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeTranslation(ELFloat tx, ELFloat ty, ELFloat tz)
{
    ELMatrix4 m = ELMatrix4Identity;
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeScale(ELFloat sx, ELFloat sy, ELFloat sz)
{
    ELMatrix4 m = ELMatrix4Identity;
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeRotation(ELFloat radians, ELFloat x, ELFloat y, ELFloat z)
{
    ELVector3 v = ELVector3Normalize(ELVector3Make(x, y, z));
    ELFloat cos = cosf(radians);
    ELFloat cosp = 1.0f - cos;
    ELFloat sin = sinf(radians);

    ELMatrix4 m = { cos + cosp * v.v[0] * v.v[0],
                     cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                     cosp * v.v[0] * v.v[2] - v.v[1] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                     cos + cosp * v.v[1] * v.v[1],
                     cosp * v.v[1] * v.v[2] + v.v[0] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                     cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                     cos + cosp * v.v[2] * v.v[2],
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeXRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix4 m = { 1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, cos, sin, 0.0f,
                     0.0f, -sin, cos, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeYRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix4 m = { cos, 0.0f, -sin, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     sin, 0.0f, cos, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeZRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix4 m = { cos, sin, 0.0f, 0.0f,
                     -sin, cos, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakePerspective(ELFloat fovyRadians, ELFloat aspect, ELFloat nearZ, ELFloat farZ)
{
    ELFloat cotan = 1.0f / tanf(fovyRadians / 2.0f);

    ELMatrix4 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
                     0.0f, cotan, 0.0f, 0.0f,
                     0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
                     0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeFrustum(ELFloat left, ELFloat right,
                                            ELFloat bottom, ELFloat top,
                                            ELFloat nearZ, ELFloat farZ)
{
    ELFloat ral = right + left;
    ELFloat rsl = right - left;
    ELFloat tsb = top - bottom;
    ELFloat tab = top + bottom;
    ELFloat fan = farZ + nearZ;
    ELFloat fsn = farZ - nearZ;

    ELMatrix4 m = { 2.0f * nearZ / rsl, 0.0f, 0.0f, 0.0f,
                     0.0f, 2.0f * nearZ / tsb, 0.0f, 0.0f,
                     ral / rsl, tab / tsb, -fan / fsn, -1.0f,
                     0.0f, 0.0f, (-2.0f * farZ * nearZ) / fsn, 0.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeOrtho(ELFloat left, ELFloat right,
                                          ELFloat bottom, ELFloat top,
                                          ELFloat nearZ, ELFloat farZ)
{
    ELFloat ral = right + left;
    ELFloat rsl = right - left;
    ELFloat tab = top + bottom;
    ELFloat tsb = top - bottom;
    ELFloat fan = farZ + nearZ;
    ELFloat fsn = farZ - nearZ;

    ELMatrix4 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
                     0.0f, 2.0f / tsb, 0.0f, 0.0f,
                     0.0f, 0.0f, -2.0f / fsn, 0.0f,
                     -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };

    return m;
}

EL_INLINE ELMatrix4 ELMatrix4MakeLookAt(ELFloat eyeX, ELFloat eyeY, ELFloat eyeZ,
                                           ELFloat centerX, ELFloat centerY, ELFloat centerZ,
                                           ELFloat upX, ELFloat upY, ELFloat upZ)
{
    ELVector3 ev = { eyeX, eyeY, eyeZ };
    ELVector3 cv = { centerX, centerY, centerZ };
    ELVector3 uv = { upX, upY, upZ };
    ELVector3 n = ELVector3Normalize(ELVector3Add(ev, ELVector3Negate(cv)));
    ELVector3 u = ELVector3Normalize(ELVector3CrossProduct(uv, n));
    ELVector3 v = ELVector3CrossProduct(n, u);

    ELMatrix4 m = { u.v[0], v.v[0], n.v[0], 0.0f,
                     u.v[1], v.v[1], n.v[1], 0.0f,
                     u.v[2], v.v[2], n.v[2], 0.0f,
                     ELVector3DotProduct(ELVector3Negate(u), ev),
                     ELVector3DotProduct(ELVector3Negate(v), ev),
                     ELVector3DotProduct(ELVector3Negate(n), ev),
                     1.0f };

    return m;
}

EL_INLINE ELMatrix3 ELMatrix4GetMatrix3(ELMatrix4 matrix)
{
ELMatrix3 m = { matrix.m[0], matrix.m[1], matrix.m[2],
                 matrix.m[4], matrix.m[5], matrix.m[6],
                 matrix.m[8], matrix.m[9], matrix.m[10] };
return m;
}

EL_INLINE ELMatrix2 ELMatrix4GetMatrix2(ELMatrix4 matrix)
{
ELMatrix2 m = { matrix.m[0], matrix.m[1],
                 matrix.m[4], matrix.m[5] };
return m;
}

EL_INLINE ELVector4 ELMatrix4GetRow(ELMatrix4 matrix, int row)
{
ELVector4 v = { matrix.m[row], matrix.m[4 + row], matrix.m[8 + row], matrix.m[12 + row] };
return v;
}

EL_INLINE ELVector4 ELMatrix4GetColumn(ELMatrix4 matrix, int column)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4_t v = vld1q_f32(&(matrix.m[column * 4]));
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
__m128 v = _mm_load_ps(&matrix.m[column * 4]);
    return *(ELVector4 *)&v;
#else
ELVector4 v = { matrix.m[column * 4 + 0], matrix.m[column * 4 + 1], matrix.m[column * 4 + 2], matrix.m[column * 4 + 3] };
return v;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4SetRow(ELMatrix4 matrix, int row, ELVector4 vector)
{
matrix.m[row] = vector.v[0];
matrix.m[row + 4] = vector.v[1];
matrix.m[row + 8] = vector.v[2];
matrix.m[row + 12] = vector.v[3];

return matrix;
}

EL_INLINE ELMatrix4 ELMatrix4SetColumn(ELMatrix4 matrix, int column, ELVector4 vector)
{
#if defined(__ARM_NEON__TODO)
ELFloat *dst = &(matrix.m[column * 4]);
    vst1q_f32(dst, vld1q_f32(vector.v));
    return matrix;
#elif defined(EL_SSE3_INTRINSICS)
*((__m128*)&matrix.m[column*4]) = *(__m128*)&vector;
    return matrix;
#else
matrix.m[column * 4 + 0] = vector.v[0];
matrix.m[column * 4 + 1] = vector.v[1];
matrix.m[column * 4 + 2] = vector.v[2];
matrix.m[column * 4 + 3] = vector.v[3];

return matrix;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4Transpose(ELMatrix4 matrix)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t m = vld4q_f32(matrix.m);
    return *(ELMatrix4 *)&m;
#else
ELMatrix4 m = { matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12],
                 matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13],
                 matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14],
                 matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15] };
return m;
#endif
}

static ELMatrix4 ELMatrix4Invert(ELMatrix4 matrix, bool * __nullable isInvertible) {
    double inv[16], det;
    int i;
    double *m = (double *)matrix.m;
    
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
    
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
    
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
    
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
    
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
    
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
    
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
    
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
    
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
    
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
    
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
    
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
    
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
    
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
    
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
    
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
    
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if (det == 0.0) {
        *isInvertible = false;
    }
    
    det = 1.0 / det;
    
    float invOut[16];
    for (i = 0; i < 16; i++) {
        invOut[i] = inv[i] * det;
    }
    
    *isInvertible = true;
    return ELMatrix4MakeWithArray(invOut);
}

EL_INLINE ELMatrix4 ELMatrix4Multiply(ELMatrix4 matrixLeft, ELMatrix4 matrixRight)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrixLeft = *(ELFloat32x4x4_t *)&matrixLeft;
    ELFloat32x4x4_t iMatrixRight = *(ELFloat32x4x4_t *)&matrixRight;
    ELFloat32x4x4_t m;

    m.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    m.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
    m.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
    m.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));

    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));

    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)

const __m128 l0 = _mm_load_ps(&matrixLeft.m[0]);
	const __m128 l1 = _mm_load_ps(&matrixLeft.m[4]);
	const __m128 l2 = _mm_load_ps(&matrixLeft.m[8]);
	const __m128 l3 = _mm_load_ps(&matrixLeft.m[12]);

	const __m128 r0 = _mm_load_ps(&matrixRight.m[0]);
	const __m128 r1 = _mm_load_ps(&matrixRight.m[4]);
	const __m128 r2 = _mm_load_ps(&matrixRight.m[8]);
	const __m128 r3 = _mm_load_ps(&matrixRight.m[12]);
	
	const __m128 m0 = l0 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0))
					+ l1 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(1, 1, 1, 1))
					+ l2 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(2, 2, 2, 2))
					+ l3 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(3, 3, 3, 3));

	const __m128 m1 = l0 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 0, 0, 0))
					+ l1 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1))
					+ l2 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 2, 2, 2))
					+ l3 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 3, 3));

	const __m128 m2 = l0 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 0, 0))
					+ l1 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 1, 1, 1))
					+ l2 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2))
					+ l3 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 3, 3, 3));

	const __m128 m3 = l0 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(0, 0, 0, 0))
					+ l1 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(1, 1, 1, 1))
					+ l2 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(2, 2, 2, 2))
					+ l3 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(3, 3, 3, 3));
				
	ELMatrix4 m;
	_mm_store_ps(&m.m[0], m0);
	_mm_store_ps(&m.m[4], m1);
	_mm_store_ps(&m.m[8], m2);
	_mm_store_ps(&m.m[12], m3);
    return m;

#else
ELMatrix4 m;

m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];

return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4Add(ELMatrix4 matrixLeft, ELMatrix4 matrixRight)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrixLeft = *(ELFloat32x4x4_t *)&matrixLeft;
    ELFloat32x4x4_t iMatrixRight = *(ELFloat32x4x4_t *)&matrixRight;
    ELFloat32x4x4_t m;
    
    m.val[0] = vaddq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vaddq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vaddq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vaddq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
    
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  + _mm_load_ps(&matrixRight.m[0]));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  + _mm_load_ps(&matrixRight.m[4]));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  + _mm_load_ps(&matrixRight.m[8]));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) + _mm_load_ps(&matrixRight.m[12]));
	    
    return m;
#else
ELMatrix4 m;

m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
m.m[2] = matrixLeft.m[2] + matrixRight.m[2];
m.m[3] = matrixLeft.m[3] + matrixRight.m[3];

m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
m.m[5] = matrixLeft.m[5] + matrixRight.m[5];
m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
m.m[7] = matrixLeft.m[7] + matrixRight.m[7];

m.m[8] = matrixLeft.m[8] + matrixRight.m[8];
m.m[9] = matrixLeft.m[9] + matrixRight.m[9];
m.m[10] = matrixLeft.m[10] + matrixRight.m[10];
m.m[11] = matrixLeft.m[11] + matrixRight.m[11];

m.m[12] = matrixLeft.m[12] + matrixRight.m[12];
m.m[13] = matrixLeft.m[13] + matrixRight.m[13];
m.m[14] = matrixLeft.m[14] + matrixRight.m[14];
m.m[15] = matrixLeft.m[15] + matrixRight.m[15];

return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4Subtract(ELMatrix4 matrixLeft, ELMatrix4 matrixRight)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrixLeft = *(ELFloat32x4x4_t *)&matrixLeft;
    ELFloat32x4x4_t iMatrixRight = *(ELFloat32x4x4_t *)&matrixRight;
    ELFloat32x4x4_t m;
    
    m.val[0] = vsubq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vsubq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vsubq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vsubq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
    
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  - _mm_load_ps(&matrixRight.m[0]));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  - _mm_load_ps(&matrixRight.m[4]));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  - _mm_load_ps(&matrixRight.m[8]));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) - _mm_load_ps(&matrixRight.m[12]));
	    
    return m;
#else
ELMatrix4 m;

m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
m.m[2] = matrixLeft.m[2] - matrixRight.m[2];
m.m[3] = matrixLeft.m[3] - matrixRight.m[3];

m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
m.m[5] = matrixLeft.m[5] - matrixRight.m[5];
m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
m.m[7] = matrixLeft.m[7] - matrixRight.m[7];

m.m[8] = matrixLeft.m[8] - matrixRight.m[8];
m.m[9] = matrixLeft.m[9] - matrixRight.m[9];
m.m[10] = matrixLeft.m[10] - matrixRight.m[10];
m.m[11] = matrixLeft.m[11] - matrixRight.m[11];

m.m[12] = matrixLeft.m[12] - matrixRight.m[12];
m.m[13] = matrixLeft.m[13] - matrixRight.m[13];
m.m[14] = matrixLeft.m[14] - matrixRight.m[14];
m.m[15] = matrixLeft.m[15] - matrixRight.m[15];

return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4Translate(ELMatrix4 matrix, ELFloat tx, ELFloat ty, ELFloat tz)
{
ELMatrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                 matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                 matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                 matrix.m[0] * tx + matrix.m[4] * ty + matrix.m[8] * tz + matrix.m[12],
                 matrix.m[1] * tx + matrix.m[5] * ty + matrix.m[9] * tz + matrix.m[13],
                 matrix.m[2] * tx + matrix.m[6] * ty + matrix.m[10] * tz + matrix.m[14],
                 matrix.m[3] * tx + matrix.m[7] * ty + matrix.m[11] * tz + matrix.m[15] };
return m;
}

EL_INLINE ELMatrix4 ELMatrix4TranslateWithVector3(ELMatrix4 matrix, ELVector3 translationVector)
{
ELMatrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                 matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                 matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                 matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
                 matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
                 matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
                 matrix.m[3] * translationVector.v[0] + matrix.m[7] * translationVector.v[1] + matrix.m[11] * translationVector.v[2] + matrix.m[15] };
return m;
}

EL_INLINE ELMatrix4 ELMatrix4TranslateWithVector4(ELMatrix4 matrix, ELVector4 translationVector)
{
ELMatrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
                 matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
                 matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
                 matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
                 matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
                 matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
                 matrix.m[3] * translationVector.v[0] + matrix.m[7] * translationVector.v[1] + matrix.m[11] * translationVector.v[2] + matrix.m[15] };
return m;
}

EL_INLINE ELMatrix4 ELMatrix4Scale(ELMatrix4 matrix, ELFloat sx, ELFloat sy, ELFloat sz)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrix = *(ELFloat32x4x4_t *)&matrix;
    ELFloat32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrix.val[0], (ELFloat32_t)sx);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (ELFloat32_t)sy);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (ELFloat32_t)sz);
    m.val[3] = iMatrix.val[3];
    
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&sx));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&sy));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&sz));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));
	    
    return m;
#else
ELMatrix4 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx, matrix.m[3] * sx,
                 matrix.m[4] * sy, matrix.m[5] * sy, matrix.m[6] * sy, matrix.m[7] * sy,
                 matrix.m[8] * sz, matrix.m[9] * sz, matrix.m[10] * sz, matrix.m[11] * sz,
                 matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4ScaleWithVector3(ELMatrix4 matrix, ELVector3 scaleVector)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrix = *(ELFloat32x4x4_t *)&matrix;
    ELFloat32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrix.val[0], (ELFloat32_t)scaleVector.v[0]);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (ELFloat32_t)scaleVector.v[1]);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (ELFloat32_t)scaleVector.v[2]);
    m.val[3] = iMatrix.val[3];
    
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&scaleVector.v[0]));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&scaleVector.v[1]));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&scaleVector.v[2]));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));
	    
    return m;
#else
ELMatrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
                 matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
                 matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
                 matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4ScaleWithVector4(ELMatrix4 matrix, ELVector4 scaleVector)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrix = *(ELFloat32x4x4_t *)&matrix;
    ELFloat32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrix.val[0], (ELFloat32_t)scaleVector.v[0]);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (ELFloat32_t)scaleVector.v[1]);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (ELFloat32_t)scaleVector.v[2]);
    m.val[3] = iMatrix.val[3];
    
    return *(ELMatrix4 *)&m;
#elif defined(EL_SSE3_INTRINSICS)
ELMatrix4 m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&scaleVector.v[0]));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&scaleVector.v[1]));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&scaleVector.v[2]));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));
	    
    return m;
#else
ELMatrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
                 matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
                 matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
                 matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
return m;
#endif
}

EL_INLINE ELMatrix4 ELMatrix4Rotate(ELMatrix4 matrix, ELFloat radians, ELFloat x, ELFloat y, ELFloat z)
{
ELMatrix4 rm = ELMatrix4MakeRotation(radians, x, y, z);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELMatrix4 ELMatrix4RotateWithVector3(ELMatrix4 matrix, ELFloat radians, ELVector3 axisVector)
{
ELMatrix4 rm = ELMatrix4MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELMatrix4 ELMatrix4RotateWithVector4(ELMatrix4 matrix, ELFloat radians, ELVector4 axisVector)
{
ELMatrix4 rm = ELMatrix4MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELMatrix4 ELMatrix4RotateX(ELMatrix4 matrix, ELFloat radians)
{
ELMatrix4 rm = ELMatrix4MakeXRotation(radians);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELMatrix4 ELMatrix4RotateY(ELMatrix4 matrix, ELFloat radians)
{
ELMatrix4 rm = ELMatrix4MakeYRotation(radians);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELMatrix4 ELMatrix4RotateZ(ELMatrix4 matrix, ELFloat radians)
{
ELMatrix4 rm = ELMatrix4MakeZRotation(radians);
return ELMatrix4Multiply(matrix, rm);
}

EL_INLINE ELVector3 ELMatrix4MultiplyVector3(ELMatrix4 matrixLeft, ELVector3 vectorRight)
{
ELVector4 v4 = ELMatrix4MultiplyVector4(matrixLeft, ELVector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 0.0f));
return ELVector3Make(v4.v[0], v4.v[1], v4.v[2]);
}

EL_INLINE ELVector3 ELMatrix4MultiplyVector3WithTranslation(ELMatrix4 matrixLeft, ELVector3 vectorRight)
{
ELVector4 v4 = ELMatrix4MultiplyVector4(matrixLeft, ELVector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
return ELVector3Make(v4.v[0], v4.v[1], v4.v[2]);
}

EL_INLINE ELVector3 ELMatrix4MultiplyAndProjectVector3(ELMatrix4 matrixLeft, ELVector3 vectorRight)
{
ELVector4 v4 = ELMatrix4MultiplyVector4(matrixLeft, ELVector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
return ELVector3MultiplyScalar(ELVector3Make(v4.v[0], v4.v[1], v4.v[2]), 1.0f / v4.v[3]);
}

EL_INLINE void ELMatrix4MultiplyVector3Array(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount)
{
int i;
for (i=0; i < vectorCount; i++)
vectors[i] = ELMatrix4MultiplyVector3(matrix, vectors[i]);
}

EL_INLINE void ELMatrix4MultiplyVector3ArrayWithTranslation(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount)
{
int i;
for (i=0; i < vectorCount; i++)
vectors[i] = ELMatrix4MultiplyVector3WithTranslation(matrix, vectors[i]);
}

EL_INLINE void ELMatrix4MultiplyAndProjectVector3Array(ELMatrix4 matrix, ELVector3 * vectors, ELSize vectorCount)
{
int i;
for (i=0; i < vectorCount; i++)
vectors[i] = ELMatrix4MultiplyAndProjectVector3(matrix, vectors[i]);
}

EL_INLINE ELVector4 ELMatrix4MultiplyVector4(ELMatrix4 matrixLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
ELFloat32x4x4_t iMatrix = *(ELFloat32x4x4_t *)&matrixLeft;
    ELFloat32x4_t v;
    
    iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (ELFloat32_t)vectorRight.v[0]);
    iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (ELFloat32_t)vectorRight.v[1]);
    iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (ELFloat32_t)vectorRight.v[2]);
    iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (ELFloat32_t)vectorRight.v[3]);
    
    iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
    iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);
    
    v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);
    
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
const __m128 v = _mm_load_ps(&vectorRight.v[0]);

	const __m128 r = _mm_load_ps(&matrixLeft.m[0])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))
	               + _mm_load_ps(&matrixLeft.m[4])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))
	               + _mm_load_ps(&matrixLeft.m[8])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))
	               + _mm_load_ps(&matrixLeft.m[12]) * _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
    
	ELVector4 ret;
	*(__m128*)&ret = r;
    return ret;
#else
ELVector4 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
                 matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
                 matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
                 matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
return v;
#endif
}

EL_INLINE void ELMatrix4MultiplyVector4Array(ELMatrix4 matrix, ELVector4 * vectors, ELSize vectorCount)
{
int i;
for (i=0; i < vectorCount; i++)
vectors[i] = ELMatrix4MultiplyVector4(matrix, vectors[i]);
}



#endif //EZGL_EZGLMATRIX4_H
