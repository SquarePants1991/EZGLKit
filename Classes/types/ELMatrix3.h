//
// Created by wangyang on 16/11/23.
//

#ifndef EZGL_ELMATRIX3_H
#define EZGL_ELMATRIX3_H

#include "EZGLBasicTypes.h"
#include "EZGLMatrixTypes.h"
#include "EZGLVectorTypes.h"

const ELMatrix3 ELMatrix3Identity = {
        1,0,0,
        0,1,0,
        0,0,1
};

EL_INLINE ELMatrix3 ELMatrix3Make(ELFloat m00, ELFloat m01, ELFloat m02,
                                     ELFloat m10, ELFloat m11, ELFloat m12,
                                     ELFloat m20, ELFloat m21, ELFloat m22);

EL_INLINE ELMatrix3 ELMatrix3MakeAndTranspose(ELFloat m00, ELFloat m01, ELFloat m02,
                                                 ELFloat m10, ELFloat m11, ELFloat m12,
                                                 ELFloat m20, ELFloat m21, ELFloat m22);

EL_INLINE ELMatrix3 ELMatrix3MakeWithArray(ELFloat values[9]);

EL_INLINE ELMatrix3 ELMatrix3MakeWithArrayAndTranspose(ELFloat values[9]);

EL_INLINE ELMatrix3 ELMatrix3MakeWithRows(ELVector3 row0,
ELVector3 row1,
        ELVector3 row2);

EL_INLINE ELMatrix3 ELMatrix3MakeWithColumns(ELVector3 column0,
ELVector3 column1,
        ELVector3 column2);

/*
 The quaternion will be normalized before conversion.
 */
EL_INLINE ELMatrix3 ELMatrix3MakeWithQuaternion(ELQuaternion quaternion);

EL_INLINE ELMatrix3 ELMatrix3MakeScale(ELFloat sx, ELFloat sy, ELFloat sz);
EL_INLINE ELMatrix3 ELMatrix3MakeRotation(ELFloat radians, ELFloat x, ELFloat y, ELFloat z);

EL_INLINE ELMatrix3 ELMatrix3MakeXRotation(ELFloat radians);
EL_INLINE ELMatrix3 ELMatrix3MakeYRotation(ELFloat radians);
EL_INLINE ELMatrix3 ELMatrix3MakeZRotation(ELFloat radians);

/*
 Returns the upper leftVector 2x2 portion of the 3x3 matrix.
 */
EL_INLINE ELMatrix2 ELMatrix3GetMatrix2(ELMatrix3 matrix);

EL_INLINE ELVector3 ELMatrix3GetRow(ELMatrix3 matrix, int row);
EL_INLINE ELVector3 ELMatrix3GetColumn(ELMatrix3 matrix, int column);

EL_INLINE ELMatrix3 ELMatrix3SetRow(ELMatrix3 matrix, int row, ELVector3 vector);
EL_INLINE ELMatrix3 ELMatrix3SetColumn(ELMatrix3 matrix, int column, ELVector3 vector);

EL_INLINE ELMatrix3 ELMatrix3Transpose(ELMatrix3 matrix);

ELMatrix3 ELMatrix3Invert(ELMatrix3 matrix, bool *isInvertible);
ELMatrix3 ELMatrix3InvertAndTranspose(ELMatrix3 matrix, bool *isInvertible);

EL_INLINE ELMatrix3 ELMatrix3Multiply(ELMatrix3 matrixLeft, ELMatrix3 matrixRight);

EL_INLINE ELMatrix3 ELMatrix3Add(ELMatrix3 matrixLeft, ELMatrix3 matrixRight);
EL_INLINE ELMatrix3 ELMatrix3Subtract(ELMatrix3 matrixLeft, ELMatrix3 matrixRight);

EL_INLINE ELMatrix3 ELMatrix3Scale(ELMatrix3 matrix, ELFloat sx, ELFloat sy, ELFloat sz);
EL_INLINE ELMatrix3 ELMatrix3ScaleWithVector3(ELMatrix3 matrix, ELVector3 scaleVector);
/*
 The last component of the ELVector4, scaleVector, is ignored.
 */
EL_INLINE ELMatrix3 ELMatrix3ScaleWithVector4(ELMatrix3 matrix, ELVector4 scaleVector);

EL_INLINE ELMatrix3 ELMatrix3Rotate(ELMatrix3 matrix, ELFloat radians, ELFloat x, ELFloat y, ELFloat z);
EL_INLINE ELMatrix3 ELMatrix3RotateWithVector3(ELMatrix3 matrix, ELFloat radians, ELVector3 axisVector);
/*
 The last component of the ELVector4, axisVector, is ignored.
 */
EL_INLINE ELMatrix3 ELMatrix3RotateWithVector4(ELMatrix3 matrix, ELFloat radians, ELVector4 axisVector);

EL_INLINE ELMatrix3 ELMatrix3RotateX(ELMatrix3 matrix, ELFloat radians);
EL_INLINE ELMatrix3 ELMatrix3RotateY(ELMatrix3 matrix, ELFloat radians);
EL_INLINE ELMatrix3 ELMatrix3RotateZ(ELMatrix3 matrix, ELFloat radians);

EL_INLINE ELVector3 ELMatrix3MultiplyVector3(ELMatrix3 matrixLeft, ELVector3 vectorRight);

EL_INLINE void ELMatrix3MultiplyVector3Array(ELMatrix3 matrix, ELVector3 * vectors, ELSize vectorCount);



EL_INLINE ELMatrix3 ELMatrix3Make(ELFloat m00, ELFloat m01, ELFloat m02,
                                     ELFloat m10, ELFloat m11, ELFloat m12,
                                     ELFloat m20, ELFloat m21, ELFloat m22)
{
    ELMatrix3 m = { m00, m01, m02,
                     m10, m11, m12,
                     m20, m21, m22 };
    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeAndTranspose(ELFloat m00, ELFloat m01, ELFloat m02,
                                                 ELFloat m10, ELFloat m11, ELFloat m12,
                                                 ELFloat m20, ELFloat m21, ELFloat m22)
{
    ELMatrix3 m = { m00, m10, m20,
                     m01, m11, m21,
                     m02, m12, m22 };
    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeWithArray(ELFloat values[9])
{
    ELMatrix3 m = { values[0], values[1], values[2],
                     values[3], values[4], values[5],
                     values[6], values[7], values[8] };
    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeWithArrayAndTranspose(ELFloat values[9])
{
    ELMatrix3 m = { values[0], values[3], values[6],
                     values[1], values[4], values[7],
                     values[2], values[5], values[8] };
    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeWithRows(ELVector3 row0,
ELVector3 row1,
        ELVector3 row2)
{
ELMatrix3 m = { row0.v[0], row1.v[0], row2.v[0],
                 row0.v[1], row1.v[1], row2.v[1],
                 row0.v[2], row1.v[2], row2.v[2] };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeWithColumns(ELVector3 column0,
ELVector3 column1,
        ELVector3 column2)
{
ELMatrix3 m = { column0.v[0], column0.v[1], column0.v[2],
                 column1.v[0], column1.v[1], column1.v[2],
                 column2.v[0], column2.v[1], column2.v[2] };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeWithQuaternion(ELQuaternion quaternion)
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

ELMatrix3 m = { 1.0f - _2y * y - _2z * z,
                 _2x * y + _2w * z,
                 _2x * z - _2w * y,

                 _2x * y - _2w * z,
                 1.0f - _2x * x - _2z * z,
                 _2y * z + _2w * x,

                 _2x * z + _2w * y,
                 _2y * z - _2w * x,
                 1.0f - _2x * x - _2y * y };

return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeScale(ELFloat sx, ELFloat sy, ELFloat sz)
{
    ELMatrix3 m = ELMatrix3Identity;
    m.m[0] = sx;
    m.m[4] = sy;
    m.m[8] = sz;
    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeRotation(ELFloat radians, ELFloat x, ELFloat y, ELFloat z)
{
    ELVector3 v = ELVector3Normalize(ELVector3Make(x, y, z));
    ELFloat cos = cosf(radians);
    ELFloat cosp = 1.0f - cos;
    ELFloat sin = sinf(radians);

    ELMatrix3 m = { cos + cosp * v.v[0] * v.v[0],
                     cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                     cosp * v.v[0] * v.v[2] - v.v[1] * sin,

                     cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                     cos + cosp * v.v[1] * v.v[1],
                     cosp * v.v[1] * v.v[2] + v.v[0] * sin,

                     cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                     cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                     cos + cosp * v.v[2] * v.v[2] };

    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeXRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix3 m = { 1.0f, 0.0f, 0.0f,
                     0.0f, cos, sin,
                     0.0f, -sin, cos };

    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeYRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix3 m = { cos, 0.0f, -sin,
                     0.0f, 1.0f, 0.0f,
                     sin, 0.0f, cos };

    return m;
}

EL_INLINE ELMatrix3 ELMatrix3MakeZRotation(ELFloat radians)
{
    ELFloat cos = cosf(radians);
    ELFloat sin = sinf(radians);

    ELMatrix3 m = { cos, sin, 0.0f,
                     -sin, cos, 0.0f,
                     0.0f, 0.0f, 1.0f };

    return m;
}

EL_INLINE ELMatrix2 ELMatrix3GetMatrix2(ELMatrix3 matrix)
{
ELMatrix2 m = { matrix.m[0], matrix.m[1],
                 matrix.m[3], matrix.m[4] };
return m;
}

EL_INLINE ELVector3 ELMatrix3GetRow(ELMatrix3 matrix, int row)
{
ELVector3 v = { matrix.m[row], matrix.m[3 + row], matrix.m[6 + row] };
return v;
}

EL_INLINE ELVector3 ELMatrix3GetColumn(ELMatrix3 matrix, int column)
{
ELVector3 v = { matrix.m[column * 3 + 0], matrix.m[column * 3 + 1], matrix.m[column * 3 + 2] };
return v;
}

EL_INLINE ELMatrix3 ELMatrix3SetRow(ELMatrix3 matrix, int row, ELVector3 vector)
{
matrix.m[row] = vector.v[0];
matrix.m[row + 3] = vector.v[1];
matrix.m[row + 6] = vector.v[2];

return matrix;
}

EL_INLINE ELMatrix3 ELMatrix3SetColumn(ELMatrix3 matrix, int column, ELVector3 vector)
{
matrix.m[column * 3 + 0] = vector.v[0];
matrix.m[column * 3 + 1] = vector.v[1];
matrix.m[column * 3 + 2] = vector.v[2];

return matrix;
}

EL_INLINE ELMatrix3 ELMatrix3Transpose(ELMatrix3 matrix)
{
ELMatrix3 m = { matrix.m[0], matrix.m[3], matrix.m[6],
                 matrix.m[1], matrix.m[4], matrix.m[7],
                 matrix.m[2], matrix.m[5], matrix.m[8] };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3Multiply(ELMatrix3 matrixLeft, ELMatrix3 matrixRight)
{
#if defined(__ARM_NEON__TODO)
ELMatrix3 m;
    ELFloat32x4x3_t iMatrixLeft;
    ELFloat32x4x3_t iMatrixRight;
    ELFloat32x4x3_t mm;
    
    iMatrixLeft.val[0] = vld1q_f32(&matrixLeft.m[0]); // 0 1 2 3
    iMatrixLeft.val[1] = vld1q_f32(&matrixLeft.m[3]); // 3 4 5 6
    iMatrixLeft.val[2] = vld1q_f32(&matrixLeft.m[5]); // 5 6 7 8

    iMatrixRight.val[0] = vld1q_f32(&matrixRight.m[0]); // 0 1 2 3
    iMatrixRight.val[1] = vld1q_f32(&matrixRight.m[3]); // 3 4 5 6
    iMatrixRight.val[2] = vld1q_f32(&matrixRight.m[5]); // 5 6 7 8
        
    iMatrixLeft.val[2] = vextq_f32(iMatrixLeft.val[2], iMatrixLeft.val[2], 1); // 6 7 8 x
    
    mm.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    mm.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 3));
    mm.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 3));
    
    mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 2));

    mm.val[0] = vmlaq_n_f32(mm.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    mm.val[1] = vmlaq_n_f32(mm.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    mm.val[2] = vmlaq_n_f32(mm.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 3));

    memcpy(&m.m[0], (char *)&(mm.val[0]), 16);
    memcpy(&m.m[3], (char *)&(mm.val[1]), 16);
    float32x2_t vlow = vget_low_f32(mm.val[2]);
    memcpy(&m.m[6], (char *)&vlow, 8);
    m.m[8] = vgetq_lane_f32(mm.val[2], 2);
    
    return m;
#elif defined(EL_SSE3_INTRINSICS)
struct {
		ELMatrix3 m;
		char pad[16*4 - sizeof(ELMatrix3)];
	} ret;
    
    const __m128 iMatrixLeft0 = _mm_loadu_ps(&matrixLeft.m[0]); // 0 1 2 3 // unaligned load
    const __m128 iMatrixLeft1 = _mm_loadu_ps(&matrixLeft.m[3]); // 3 4 5 6 // unaligned load
    const __m128 iMatrixLeft2Tmp = _mm_loadu_ps(&matrixLeft.m[5]); // 5 6 7 8 // unaligned load
    const __m128 iMatrixLeft2 = _mm_shuffle_ps(iMatrixLeft2Tmp, iMatrixLeft2Tmp, _MM_SHUFFLE(0, 3, 2, 1)); // 6 7 8 x
    
    const __m128 iMatrixRight0 = _mm_loadu_ps(&matrixRight.m[0]);
    const __m128 iMatrixRight1 = _mm_loadu_ps(&matrixRight.m[3]);
    const __m128 iMatrixRight2 = _mm_loadu_ps(&matrixRight.m[5]);
    
    const __m128 mm0 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(0, 0, 0, 0))  // mm0 = L0*R0 L1*R0 L2*R0 L3*R0
                     + iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(1, 1, 1, 1))  // mm0 = L0*R0+L3*R1 L1*R0+L4*R1 L2*R0+L5*R1 L3*R0+L6*R1
                     + iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(2, 2, 2, 2));
		
    const __m128 mm1 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(3, 3, 3, 3))  // mm1 = L0*R3 L1*R3 L2*R3 L3*R3
                     + iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(1, 1, 1, 1))  // mm1 = L0*R3+L3*R4 L1*R3+L4*R4 L2*R3+L5*R4 L3*R3+
                     + iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(2, 2, 2, 2));
	
    const __m128 mm2 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(3, 3, 3, 3)) // mm2 = L0*R6 L1*R6 L2*R6 L3*R6
                     + iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight2, iMatrixRight2, _MM_SHUFFLE(2, 2, 2, 2))
                     + iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight2, iMatrixRight2, _MM_SHUFFLE(3, 3, 3, 3));
    
    _mm_storeu_ps(&ret.m.m[0], mm0); //unaligned store to indices: 0 1 2 3
    _mm_storeu_ps(&ret.m.m[3], mm1); //unaligned store to indices: 3 4 5 6
    _mm_storeu_ps(&ret.m.m[6], mm2); //unaligned store to indices: 6 7 8
		
    return ret.m;
#else
ELMatrix3 m;

m.m[0] = matrixLeft.m[0] * matrixRight.m[0] + matrixLeft.m[3] * matrixRight.m[1] + matrixLeft.m[6] * matrixRight.m[2];
m.m[3] = matrixLeft.m[0] * matrixRight.m[3] + matrixLeft.m[3] * matrixRight.m[4] + matrixLeft.m[6] * matrixRight.m[5];
m.m[6] = matrixLeft.m[0] * matrixRight.m[6] + matrixLeft.m[3] * matrixRight.m[7] + matrixLeft.m[6] * matrixRight.m[8];

m.m[1] = matrixLeft.m[1] * matrixRight.m[0] + matrixLeft.m[4] * matrixRight.m[1] + matrixLeft.m[7] * matrixRight.m[2];
m.m[4] = matrixLeft.m[1] * matrixRight.m[3] + matrixLeft.m[4] * matrixRight.m[4] + matrixLeft.m[7] * matrixRight.m[5];
m.m[7] = matrixLeft.m[1] * matrixRight.m[6] + matrixLeft.m[4] * matrixRight.m[7] + matrixLeft.m[7] * matrixRight.m[8];

m.m[2] = matrixLeft.m[2] * matrixRight.m[0] + matrixLeft.m[5] * matrixRight.m[1] + matrixLeft.m[8] * matrixRight.m[2];
m.m[5] = matrixLeft.m[2] * matrixRight.m[3] + matrixLeft.m[5] * matrixRight.m[4] + matrixLeft.m[8] * matrixRight.m[5];
m.m[8] = matrixLeft.m[2] * matrixRight.m[6] + matrixLeft.m[5] * matrixRight.m[7] + matrixLeft.m[8] * matrixRight.m[8];

return m;
#endif
}

EL_INLINE ELMatrix3 ELMatrix3Add(ELMatrix3 matrixLeft, ELMatrix3 matrixRight)
{
#if defined(EL_SSE3_INTRINSICS)
ELMatrix3 m;
    
    _mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) + _mm_loadu_ps(&matrixRight.m[0]));
    _mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) + _mm_loadu_ps(&matrixRight.m[4]));
    m.m[8] = matrixLeft.m[8] + matrixRight.m[8];
    
    return m;
#else
ELMatrix3 m;

m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
m.m[2] = matrixLeft.m[2] + matrixRight.m[2];

m.m[3] = matrixLeft.m[3] + matrixRight.m[3];
m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
m.m[5] = matrixLeft.m[5] + matrixRight.m[5];

m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
m.m[7] = matrixLeft.m[7] + matrixRight.m[7];
m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

return m;
#endif
}

EL_INLINE ELMatrix3 ELMatrix3Subtract(ELMatrix3 matrixLeft, ELMatrix3 matrixRight)
{
#if defined(EL_SSE3_INTRINSICS)
ELMatrix3 m;
    
    _mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) - _mm_loadu_ps(&matrixRight.m[0]));
    _mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) - _mm_loadu_ps(&matrixRight.m[4]));
    m.m[8] = matrixLeft.m[8] - matrixRight.m[8];
    
    return m;
#else
ELMatrix3 m;

m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
m.m[2] = matrixLeft.m[2] - matrixRight.m[2];

m.m[3] = matrixLeft.m[3] - matrixRight.m[3];
m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
m.m[5] = matrixLeft.m[5] - matrixRight.m[5];

m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
m.m[7] = matrixLeft.m[7] - matrixRight.m[7];
m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

return m;
#endif
}

EL_INLINE ELMatrix3 ELMatrix3Scale(ELMatrix3 matrix, ELFloat sx, ELFloat sy, ELFloat sz)
{
ELMatrix3 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx,
                 matrix.m[3] * sy, matrix.m[4] * sy, matrix.m[5] * sy,
                 matrix.m[6] * sz, matrix.m[7] * sz, matrix.m[8] * sz };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3ScaleWithVector3(ELMatrix3 matrix, ELVector3 scaleVector)
{
ELMatrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
                 matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
                 matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3ScaleWithVector4(ELMatrix3 matrix, ELVector4 scaleVector)
{
ELMatrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
                 matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
                 matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
return m;
}

EL_INLINE ELMatrix3 ELMatrix3Rotate(ELMatrix3 matrix, ELFloat radians, ELFloat x, ELFloat y, ELFloat z)
{
ELMatrix3 rm = ELMatrix3MakeRotation(radians, x, y, z);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELMatrix3 ELMatrix3RotateWithVector3(ELMatrix3 matrix, ELFloat radians, ELVector3 axisVector)
{
ELMatrix3 rm = ELMatrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELMatrix3 ELMatrix3RotateWithVector4(ELMatrix3 matrix, ELFloat radians, ELVector4 axisVector)
{
ELMatrix3 rm = ELMatrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELMatrix3 ELMatrix3RotateX(ELMatrix3 matrix, ELFloat radians)
{
ELMatrix3 rm = ELMatrix3MakeXRotation(radians);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELMatrix3 ELMatrix3RotateY(ELMatrix3 matrix, ELFloat radians)
{
ELMatrix3 rm = ELMatrix3MakeYRotation(radians);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELMatrix3 ELMatrix3RotateZ(ELMatrix3 matrix, ELFloat radians)
{
ELMatrix3 rm = ELMatrix3MakeZRotation(radians);
return ELMatrix3Multiply(matrix, rm);
}

EL_INLINE ELVector3 ELMatrix3MultiplyVector3(ELMatrix3 matrixLeft, ELVector3 vectorRight)
{
ELVector3 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[3] * vectorRight.v[1] + matrixLeft.m[6] * vectorRight.v[2],
                 matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[7] * vectorRight.v[2],
                 matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] };
return v;
}

EL_INLINE void ELMatrix3MultiplyVector3Array(ELMatrix3 matrix, ELVector3 * vectors, ELSize vectorCount)
{
int i;
for (i=0; i < vectorCount; i++)
vectors[i] = ELMatrix3MultiplyVector3(matrix, vectors[i]);
}


#endif //EZGL_ELMATRIX3_H
