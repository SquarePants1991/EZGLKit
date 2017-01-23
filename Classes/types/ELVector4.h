//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_ELVECTOR4_H
#define EZGL_ELVECTOR4_H

#include "EZGLBasicTypes.h"
#include "EZGLVectorTypes.h"

EL_INLINE ELVector4 ELVector4Make(ELFloat x, ELFloat y, ELFloat z, ELFloat w);
EL_INLINE ELVector4 ELVector4MakeWithArray(ELFloat values[4]);
EL_INLINE ELVector4 ELVector4MakeWithVector3(ELVector3 vector, ELFloat w);

EL_INLINE ELVector4 ELVector4Negate(ELVector4 vector);

EL_INLINE ELVector4 ELVector4Add(ELVector4 vectorLeft, ELVector4 vectorRight);
EL_INLINE ELVector4 ELVector4Subtract(ELVector4 vectorLeft, ELVector4 vectorRight);
EL_INLINE ELVector4 ELVector4Multiply(ELVector4 vectorLeft, ELVector4 vectorRight);
EL_INLINE ELVector4 ELVector4Divide(ELVector4 vectorLeft, ELVector4 vectorRight);

EL_INLINE ELVector4 ELVector4AddScalar(ELVector4 vector, ELFloat value);
EL_INLINE ELVector4 ELVector4SubtractScalar(ELVector4 vector, ELFloat value);
EL_INLINE ELVector4 ELVector4MultiplyScalar(ELVector4 vector, ELFloat value);
EL_INLINE ELVector4 ELVector4DivideScalar(ELVector4 vector, ELFloat value);

/*
 Returns a vector whose elements are the larger of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector4 ELVector4Maximum(ELVector4 vectorLeft, ELVector4 vectorRight);
/*
 Returns a vector whose elements are the smaller of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector4 ELVector4Minimum(ELVector4 vectorLeft, ELVector4 vectorRight);

/*
 Returns true if all of the first vector's elements are equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector4AllEqualToVector4(ELVector4 vectorLeft, ELVector4 vectorRight);
/*
 Returns true if all of the vector's elements are equal to the provided value.
 */
EL_INLINE bool ELVector4AllEqualToScalar(ELVector4 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than all of the second vector's arguments.
 */
EL_INLINE bool ELVector4AllGreaterThanVector4(ELVector4 vectorLeft, ELVector4 vectorRight);
/*
 Returns true if all of the vector's elements are greater than the provided value.
 */
EL_INLINE bool ELVector4AllGreaterThanScalar(ELVector4 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than or equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector4AllGreaterThanOrEqualToVector4(ELVector4 vectorLeft, ELVector4 vectorRight);
/*
 Returns true if all of the vector's elements are greater than or equal to the provided value.
 */
EL_INLINE bool ELVector4AllGreaterThanOrEqualToScalar(ELVector4 vector, ELFloat value);

EL_INLINE ELVector4 ELVector4Normalize(ELVector4 vector);

EL_INLINE ELFloat ELVector4DotProduct(ELVector4 vectorLeft, ELVector4 vectorRight);
EL_INLINE ELFloat ELVector4Length(ELVector4 vector);
EL_INLINE ELFloat ELVector4Distance(ELVector4 vectorStart, ELVector4 vectorEnd);

EL_INLINE ELVector4 ELVector4Lerp(ELVector4 vectorStart, ELVector4 vectorEnd, ELFloat t);

/*
 Performs a 3D cross product. The last component of the resulting cross product will be zeroed out.
 */
EL_INLINE ELVector4 ELVector4CrossProduct(ELVector4 vectorLeft, ELVector4 vectorRight);

/*
 Project the vector, vectorToProject, onto the vector, projectionVector.
 */
EL_INLINE ELVector4 ELVector4Project(ELVector4 vectorToProject, ELVector4 projectionVector);



// implements
EL_INLINE ELVector4 ELVector4Make(ELFloat x, ELFloat y, ELFloat z, ELFloat w)
{
    ELVector4 v = { x, y, z, w };
    return v;
}

EL_INLINE ELVector4 ELVector4MakeWithArray(ELFloat values[4])
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vld1q_f32(values);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(values);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { values[0], values[1], values[2], values[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4MakeWithVector3(ELVector3 vector, ELFloat w)
{
    ELVector4 v = { vector.v[0], vector.v[1], vector.v[2], w };
    return v;
}

EL_INLINE ELVector4 ELVector4Negate(ELVector4 vector)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vnegq_f32(*(ELFloat32x4_t *)&vector);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_set1_ps(0) - _mm_load_ps(&vector.v[0]);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { -vector.v[0], -vector.v[1], -vector.v[2], -vector.v[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Add(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vaddq_f32(*(ELFloat32x4_t *)&vectorLeft,
                              *(ELFloat32x4_t *)&vectorRight);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vectorLeft.v[0]) + _mm_load_ps(&vectorRight.v[0]);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vectorLeft.v[0] + vectorRight.v[0],
                    vectorLeft.v[1] + vectorRight.v[1],
                    vectorLeft.v[2] + vectorRight.v[2],
                    vectorLeft.v[3] + vectorRight.v[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Subtract(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vsubq_f32(*(ELFloat32x4_t *)&vectorLeft,
                              *(ELFloat32x4_t *)&vectorRight);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vectorLeft.v[0]) - _mm_load_ps(&vectorRight.v[0]);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vectorLeft.v[0] - vectorRight.v[0],
                    vectorLeft.v[1] - vectorRight.v[1],
                    vectorLeft.v[2] - vectorRight.v[2],
                    vectorLeft.v[3] - vectorRight.v[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Multiply(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vmulq_f32(*(ELFloat32x4_t *)&vectorLeft,
                              *(ELFloat32x4_t *)&vectorRight);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vectorLeft.v[0]) * _mm_load_ps(&vectorRight.v[0]);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vectorLeft.v[0] * vectorRight.v[0],
                    vectorLeft.v[1] * vectorRight.v[1],
                    vectorLeft.v[2] * vectorRight.v[2],
                    vectorLeft.v[3] * vectorRight.v[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Divide(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t *vLeft = (ELFloat32x4_t *)&vectorLeft;
    ELFloat32x4_t *vRight = (ELFloat32x4_t *)&vectorRight;
    ELFloat32x4_t estimate = vrecpeq_f32(*vRight);
    estimate = vmulq_f32(vrecpsq_f32(*vRight, estimate), estimate);
    estimate = vmulq_f32(vrecpsq_f32(*vRight, estimate), estimate);
    ELFloat32x4_t v = vmulq_f32(*vLeft, estimate);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vectorLeft.v[0]) / _mm_load_ps(&vectorRight.v[0]);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vectorLeft.v[0] / vectorRight.v[0],
                    vectorLeft.v[1] / vectorRight.v[1],
                    vectorLeft.v[2] / vectorRight.v[2],
                    vectorLeft.v[3] / vectorRight.v[3] };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4AddScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vaddq_f32(*(ELFloat32x4_t *)&vector,
                              vdupq_n_f32((ELFloat32_t)value));
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vector.v[0]) + _mm_set1_ps(value);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vector.v[0] + value,
                    vector.v[1] + value,
                    vector.v[2] + value,
                    vector.v[3] + value };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4SubtractScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vsubq_f32(*(ELFloat32x4_t *)&vector,
                              vdupq_n_f32((ELFloat32_t)value));
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vector.v[0]) - _mm_set1_ps(value);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vector.v[0] - value,
                    vector.v[1] - value,
                    vector.v[2] - value,
                    vector.v[3] - value };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4MultiplyScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vmulq_f32(*(ELFloat32x4_t *)&vector,
                              vdupq_n_f32((ELFloat32_t)value));
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vector.v[0]) * _mm_set1_ps(value);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vector.v[0] * value,
                    vector.v[1] * value,
                    vector.v[2] * value,
                    vector.v[3] * value };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4DivideScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t values = vdupq_n_f32((ELFloat32_t)value);
    ELFloat32x4_t estimate = vrecpeq_f32(values);
    estimate = vmulq_f32(vrecpsq_f32(values, estimate), estimate);
    estimate = vmulq_f32(vrecpsq_f32(values, estimate), estimate);
    ELFloat32x4_t v = vmulq_f32(*(ELFloat32x4_t *)&vector, estimate);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(&vector.v[0]) / _mm_set1_ps(value);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vector.v[0] / value,
                    vector.v[1] / value,
                    vector.v[2] / value,
                    vector.v[3] / value };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Maximum(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vmaxq_f32(*(ELFloat32x4_t *)&vectorLeft,
                              *(ELFloat32x4_t *)&vectorRight);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_max_ps(_mm_load_ps(&vectorLeft.v[0]), _mm_load_ps(&vectorRight.v[0]));
    return *(ELVector4 *)&v;
#else
    ELVector4 max = vectorLeft;
    if (vectorRight.v[0] > vectorLeft.v[0])
        max.v[0] = vectorRight.v[0];
    if (vectorRight.v[1] > vectorLeft.v[1])
        max.v[1] = vectorRight.v[1];
    if (vectorRight.v[2] > vectorLeft.v[2])
        max.v[2] = vectorRight.v[2];
    if (vectorRight.v[3] > vectorLeft.v[3])
        max.v[3] = vectorRight.v[3];
    return max;
#endif
}

EL_INLINE ELVector4 ELVector4Minimum(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vminq_f32(*(ELFloat32x4_t *)&vectorLeft,
                                  *(ELFloat32x4_t *)&vectorRight);
        return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_min_ps(_mm_load_ps(&vectorLeft.v[0]), _mm_load_ps(&vectorRight.v[0]));
    return *(ELVector4 *)&v;
#else
    ELVector4 min = vectorLeft;
    if (vectorRight.v[0] < vectorLeft.v[0])
        min.v[0] = vectorRight.v[0];
    if (vectorRight.v[1] < vectorLeft.v[1])
        min.v[1] = vectorRight.v[1];
    if (vectorRight.v[2] < vectorLeft.v[2])
        min.v[2] = vectorRight.v[2];
    if (vectorRight.v[3] < vectorLeft.v[3])
        min.v[3] = vectorRight.v[3];
    return min;
#endif
}

EL_INLINE bool ELVector4AllEqualToVector4(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vectorLeft;
    ELFloat32x4_t v2 = *(ELFloat32x4_t *)&vectorRight;
    uint32x4_t vCmp = vceqq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpeq_ps(_mm_load_ps(&vectorLeft.v[0]), _mm_load_ps(&vectorRight.v[0]))) == 0xF;
#else
    bool compare = false;
    if (vectorLeft.v[0] == vectorRight.v[0] &&
        vectorLeft.v[1] == vectorRight.v[1] &&
        vectorLeft.v[2] == vectorRight.v[2] &&
        vectorLeft.v[3] == vectorRight.v[3])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector4AllEqualToScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vector;
    ELFloat32x4_t v2 = vdupq_n_f32(value);
    uint32x4_t vCmp = vceqq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpeq_ps(_mm_load_ps(&vector.v[0]), _mm_set1_ps(value))) == 0xF;
#else
    bool compare = false;
    if (vector.v[0] == value &&
        vector.v[1] == value &&
        vector.v[2] == value &&
        vector.v[3] == value)
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector4AllGreaterThanVector4(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vectorLeft;
    ELFloat32x4_t v2 = *(ELFloat32x4_t *)&vectorRight;
    uint32x4_t vCmp = vcgtq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpgt_ps(_mm_load_ps(&vectorLeft.v[0]), _mm_load_ps(&vectorRight.v[0]))) == 0xF;
#else
    bool compare = false;
    if (vectorLeft.v[0] > vectorRight.v[0] &&
        vectorLeft.v[1] > vectorRight.v[1] &&
        vectorLeft.v[2] > vectorRight.v[2] &&
        vectorLeft.v[3] > vectorRight.v[3])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector4AllGreaterThanScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vector;
    ELFloat32x4_t v2 = vdupq_n_f32(value);
    uint32x4_t vCmp = vcgtq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpgt_ps(_mm_load_ps(&vector.v[0]), _mm_set1_ps(value))) == 0xF;
#else
    bool compare = false;
    if (vector.v[0] > value &&
        vector.v[1] > value &&
        vector.v[2] > value &&
        vector.v[3] > value)
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector4AllGreaterThanOrEqualToVector4(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vectorLeft;
    ELFloat32x4_t v2 = *(ELFloat32x4_t *)&vectorRight;
    uint32x4_t vCmp = vcgeq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpge_ps(_mm_load_ps(&vectorLeft.v[0]), _mm_load_ps(&vectorRight.v[0]))) == 0xF;
#else
    bool compare = false;
    if (vectorLeft.v[0] >= vectorRight.v[0] &&
        vectorLeft.v[1] >= vectorRight.v[1] &&
        vectorLeft.v[2] >= vectorRight.v[2] &&
        vectorLeft.v[3] >= vectorRight.v[3])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector4AllGreaterThanOrEqualToScalar(ELVector4 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v1 = *(ELFloat32x4_t *)&vector;
    ELFloat32x4_t v2 = vdupq_n_f32(value);
    uint32x4_t vCmp = vcgeq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vget_low_u32(vCmp), vget_high_u32(vCmp));
    vAnd = vand_u32(vAnd, vext_u32(vAnd, vAnd, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#elif defined(EL_SSE3_INTRINSICS)
    return _mm_movemask_ps(_mm_cmpge_ps(_mm_load_ps(&vector.v[0]), _mm_set1_ps(value))) == 0xF;
#else
    bool compare = false;
    if (vector.v[0] >= value &&
        vector.v[1] >= value &&
        vector.v[2] >= value &&
        vector.v[3] >= value)
        compare = true;
    return compare;
#endif
}

EL_INLINE ELVector4 ELVector4Normalize(ELVector4 vector)
{
    ELFloat scale = 1.0f / ELVector4Length(vector);
    ELVector4 v = ELVector4MultiplyScalar(vector, scale);
    return v;
}

EL_INLINE ELFloat ELVector4DotProduct(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vmulq_f32(*(ELFloat32x4_t *)&vectorLeft,
                              *(ELFloat32x4_t *)&vectorRight);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return vget_lane_f32(v2, 0);
#elif defined(EL_SSE3_INTRINSICS)
    const __m128 product = _mm_load_ps(&vectorLeft.v[0]) * _mm_load_ps(&vectorRight.v[0]);
	const __m128 halfsum = _mm_hadd_ps(product, product);
	return _mm_cvtss_f32(_mm_hadd_ps(halfsum, halfsum));
#else
    return vectorLeft.v[0] * vectorRight.v[0] +
           vectorLeft.v[1] * vectorRight.v[1] +
           vectorLeft.v[2] * vectorRight.v[2] +
           vectorLeft.v[3] * vectorRight.v[3];
#endif
}

EL_INLINE ELFloat ELVector4Length(ELVector4 vector)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t v = vmulq_f32(*(ELFloat32x4_t *)&vector,
                              *(ELFloat32x4_t *)&vector);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return sqrt(vget_lane_f32(v2, 0));
#elif defined(EL_SSE3_INTRINSICS)
    const __m128 q = _mm_load_ps(&vector.v[0]);
	const __m128 product = q * q;
	const __m128 halfsum = _mm_hadd_ps(product, product);
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(halfsum, halfsum)));
#else
    return sqrt(vector.v[0] * vector.v[0] +
                vector.v[1] * vector.v[1] +
                vector.v[2] * vector.v[2] +
                vector.v[3] * vector.v[3]);
#endif
}

EL_INLINE ELFloat ELVector4Distance(ELVector4 vectorStart, ELVector4 vectorEnd)
{
    return ELVector4Length(ELVector4Subtract(vectorEnd, vectorStart));
}

EL_INLINE ELVector4 ELVector4Lerp(ELVector4 vectorStart, ELVector4 vectorEnd, ELFloat t)
{
#if defined(__ARM_NEON__TODO)
    ELFloat32x4_t vDiff = vsubq_f32(*(ELFloat32x4_t *)&vectorEnd,
                                  *(ELFloat32x4_t *)&vectorStart);
    vDiff = vmulq_f32(vDiff, vdupq_n_f32((ELFloat32_t)t));
    ELFloat32x4_t v = vaddq_f32(*(ELFloat32x4_t *)&vectorStart, vDiff);
    return *(ELVector4 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    const __m128 s =  _mm_load_ps(&vectorStart.v[0]);
    const __m128 v = s + (_mm_load_ps(&vectorEnd.v[0]) - s) * _mm_set1_ps(t);
    return *(ELVector4 *)&v;
#else
    ELVector4 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
                    vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
                    vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t),
                    vectorStart.v[3] + ((vectorEnd.v[3] - vectorStart.v[3]) * t) };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4CrossProduct(ELVector4 vectorLeft, ELVector4 vectorRight)
{
#if defined(EL_SSE3_INTRINSICS)
    const __m128 vl = _mm_load_ps(&vectorLeft.v[0]);
	const __m128 vr = _mm_load_ps(&vectorRight.v[0]);

    __m128 vLeft1  = _mm_shuffle_ps(vl, vl, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 vRight1 = _mm_shuffle_ps(vr, vr, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 vLeft2  = _mm_shuffle_ps(vl, vl, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 vRight2 = _mm_shuffle_ps(vr, vr, _MM_SHUFFLE(3, 0, 2, 1));
    vLeft1 = vLeft1 * vRight1;
    vLeft2 = vLeft2 * vRight2;
    vLeft1 = vLeft1 - vLeft2;
    //Set last element to 0
    uint32_t mask[4] __attribute__((aligned(16))) = {0xffffffff, 0xffffffff, 0xffffffff, 0x0};
    vLeft1 = _mm_and_ps(vLeft1, _mm_load_ps((ELFloat *)mask));
    return *(ELVector4 *)&vLeft1;
#else
    ELVector4 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
                    vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
                    vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0],
                    0.0f };
    return v;
#endif
}

EL_INLINE ELVector4 ELVector4Project(ELVector4 vectorToProject, ELVector4 projectionVector)
{
    ELFloat scale = ELVector4DotProduct(projectionVector, vectorToProject) / ELVector4DotProduct(projectionVector, projectionVector);
    ELVector4 v = ELVector4MultiplyScalar(projectionVector, scale);
    return v;
}

#endif //EZGL_ELVECTOR4_H
