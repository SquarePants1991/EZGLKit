//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_ELVECTOR2_H
#define EZGL_ELVECTOR2_H

#include "EZGLBasicTypes.h"
#include "EZGLVectorTypes.h"

EL_INLINE ELVector2 ELVector2Make(ELFloat x, ELFloat y);
EL_INLINE ELVector2 ELVector2MakeWithArray(ELFloat values[2]);

EL_INLINE ELVector2 ELVector2Negate(ELVector2 vector);

EL_INLINE ELVector2 ELVector2Add(ELVector2 vectorLeft, ELVector2 vectorRight);
EL_INLINE ELVector2 ELVector2Subtract(ELVector2 vectorLeft, ELVector2 vectorRight);
EL_INLINE ELVector2 ELVector2Multiply(ELVector2 vectorLeft, ELVector2 vectorRight);
EL_INLINE ELVector2 ELVector2Divide(ELVector2 vectorLeft, ELVector2 vectorRight);

EL_INLINE ELVector2 ELVector2AddScalar(ELVector2 vector, ELFloat value);
EL_INLINE ELVector2 ELVector2SubtractScalar(ELVector2 vector, ELFloat value);
EL_INLINE ELVector2 ELVector2MultiplyScalar(ELVector2 vector, ELFloat value);
EL_INLINE ELVector2 ELVector2DivideScalar(ELVector2 vector, ELFloat value);

/*
 Returns a vector whose elements are the larger of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector2 ELVector2Maximum(ELVector2 vectorLeft, ELVector2 vectorRight);
/*
 Returns a vector whose elements are the smaller of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector2 ELVector2Minimum(ELVector2 vectorLeft, ELVector2 vectorRight);

/*
 Returns true if all of the first vector's elements are equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector2AllEqualToVector2(ELVector2 vectorLeft, ELVector2 vectorRight);
/*
 Returns true if all of the vector's elements are equal to the provided value.
 */
EL_INLINE bool ELVector2AllEqualToScalar(ELVector2 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than all of the second vector's arguments.
 */
EL_INLINE bool ELVector2AllGreaterThanVector2(ELVector2 vectorLeft, ELVector2 vectorRight);
/*
 Returns true if all of the vector's elements are greater than the provided value.
 */
EL_INLINE bool ELVector2AllGreaterThanScalar(ELVector2 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than or equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector2AllGreaterThanOrEqualToVector2(ELVector2 vectorLeft, ELVector2 vectorRight);
/*
 Returns true if all of the vector's elements are greater than or equal to the provided value.
 */
EL_INLINE bool ELVector2AllGreaterThanOrEqualToScalar(ELVector2 vector, ELFloat value);

EL_INLINE ELVector2 ELVector2Normalize(ELVector2 vector);

EL_INLINE ELFloat ELVector2DotProduct(ELVector2 vectorLeft, ELVector2 vectorRight);
EL_INLINE ELFloat ELVector2Length(ELVector2 vector);
EL_INLINE ELFloat ELVector2Distance(ELVector2 vectorStart, ELVector2 vectorEnd);

EL_INLINE ELVector2 ELVector2Lerp(ELVector2 vectorStart, ELVector2 vectorEnd, ELFloat t);

/*
 Project the vector, vectorToProject, onto the vector, projectionVector.
 */
EL_INLINE ELVector2 ELVector2Project(ELVector2 vectorToProject, ELVector2 projectionVector);

EL_INLINE ELVector2 ELVector2Make(ELFloat x, ELFloat y)
{
    ELVector2 v = { x, y };
    return v;
}

EL_INLINE ELVector2 ELVector2MakeWithArray(ELFloat values[2])
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vld1_f32(values);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_loadl_pi(v, (__m64 *)values);
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { values[0], values[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Negate(ELVector2 vector)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vneg_f32(*(float32x2_t *)&vector);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_sub_ps(_mm_setzero_ps(), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vector));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { -vector.v[0] , -vector.v[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Add(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vadd_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_add_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vectorLeft.v[0] + vectorRight.v[0],
                    vectorLeft.v[1] + vectorRight.v[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Subtract(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vsub_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_sub_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vectorLeft.v[0] - vectorRight.v[0],
                    vectorLeft.v[1] - vectorRight.v[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Multiply(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmul_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_mul_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vectorLeft.v[0] * vectorRight.v[0],
                    vectorLeft.v[1] * vectorRight.v[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Divide(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t *vLeft = (float32x2_t *)&vectorLeft;
    float32x2_t *vRight = (float32x2_t *)&vectorRight;
    float32x2_t estimate = vrecpe_f32(*vRight);
    estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
    estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
    float32x2_t v = vmul_f32(*vLeft, estimate);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_div_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vectorLeft.v[0] / vectorRight.v[0],
                    vectorLeft.v[1] / vectorRight.v[1] };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2AddScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vadd_f32(*(float32x2_t *)&vector,
                             vdup_n_f32((ELFloat32_t)value));
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_add_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vector), _mm_set1_ps(value));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vector.v[0] + value,
                    vector.v[1] + value };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2SubtractScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vsub_f32(*(float32x2_t *)&vector,
                             vdup_n_f32((ELFloat32_t)value));
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_sub_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vector), _mm_set1_ps(value));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vector.v[0] - value,
                    vector.v[1] - value };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2MultiplyScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmul_f32(*(float32x2_t *)&vector,
                             vdup_n_f32((ELFloat32_t)value));
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_mul_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vector), _mm_set1_ps(value));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vector.v[0] * value,
                    vector.v[1] * value };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2DivideScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t values = vdup_n_f32((ELFloat32_t)value);
    float32x2_t estimate = vrecpe_f32(values);
    estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
    estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
    float32x2_t v = vmul_f32(*(float32x2_t *)&vector, estimate);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_div_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vector), _mm_set1_ps(value));
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vector.v[0] / value,
                    vector.v[1] / value };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Maximum(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmax_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_max_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 max = vectorLeft;
    if (vectorRight.v[0] > vectorLeft.v[0])
        max.v[0] = vectorRight.v[0];
    if (vectorRight.v[1] > vectorLeft.v[1])
        max.v[1] = vectorRight.v[1];
    return max;
#endif
}

EL_INLINE ELVector2 ELVector2Minimum(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmin_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    return *(ELVector2 *)&v;
#elif defined(EL_SSE3_INTRINSICS)
    __m128 v;
    v = _mm_min_ps(_mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorLeft), _mm_loadl_pi(_mm_setzero_ps(), (__m64 *)&vectorRight));
    return *(ELVector2 *)&v;
#else
    ELVector2 min = vectorLeft;
    if (vectorRight.v[0] < vectorLeft.v[0])
        min.v[0] = vectorRight.v[0];
    if (vectorRight.v[1] < vectorLeft.v[1])
        min.v[1] = vectorRight.v[1];
    return min;
#endif
}

EL_INLINE bool ELVector2AllEqualToVector2(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vectorLeft;
    float32x2_t v2 = *(float32x2_t *)&vectorRight;
    uint32x2_t vCmp = vceq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vectorLeft.v[0] == vectorRight.v[0] &&
        vectorLeft.v[1] == vectorRight.v[1])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector2AllEqualToScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vector;
    float32x2_t v2 = vdup_n_f32(value);
    uint32x2_t vCmp = vceq_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vector.v[0] == value &&
        vector.v[1] == value)
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector2AllGreaterThanVector2(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vectorLeft;
    float32x2_t v2 = *(float32x2_t *)&vectorRight;
    uint32x2_t vCmp = vcgt_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vectorLeft.v[0] > vectorRight.v[0] &&
        vectorLeft.v[1] > vectorRight.v[1])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector2AllGreaterThanScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vector;
    float32x2_t v2 = vdup_n_f32(value);
    uint32x2_t vCmp = vcgt_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vector.v[0] > value &&
        vector.v[1] > value)
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector2AllGreaterThanOrEqualToVector2(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vectorLeft;
    float32x2_t v2 = *(float32x2_t *)&vectorRight;
    uint32x2_t vCmp = vcge_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vectorLeft.v[0] >= vectorRight.v[0] &&
        vectorLeft.v[1] >= vectorRight.v[1])
        compare = true;
    return compare;
#endif
}

EL_INLINE bool ELVector2AllGreaterThanOrEqualToScalar(ELVector2 vector, ELFloat value)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v1 = *(float32x2_t *)&vector;
    float32x2_t v2 = vdup_n_f32(value);
    uint32x2_t vCmp = vcge_f32(v1, v2);
    uint32x2_t vAnd = vand_u32(vCmp, vext_u32(vCmp, vCmp, 1));
    vAnd = vand_u32(vAnd, vdup_n_u32(1));
    return (bool)vget_lane_u32(vAnd, 0);
#else
    bool compare = false;
    if (vector.v[0] >= value &&
        vector.v[1] >= value)
        compare = true;
    return compare;
#endif
}

EL_INLINE ELVector2 ELVector2Normalize(ELVector2 vector)
{
    ELFloat scale = 1.0f / ELVector2Length(vector);
    ELVector2 v = ELVector2MultiplyScalar(vector, scale);
    return v;
}

EL_INLINE ELFloat ELVector2DotProduct(ELVector2 vectorLeft, ELVector2 vectorRight)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmul_f32(*(float32x2_t *)&vectorLeft,
                             *(float32x2_t *)&vectorRight);
    v = vpadd_f32(v, v);
    return vget_lane_f32(v, 0);
#else
    return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1];
#endif
}

EL_INLINE ELFloat ELVector2Length(ELVector2 vector)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t v = vmul_f32(*(float32x2_t *)&vector,
                             *(float32x2_t *)&vector);
    v = vpadd_f32(v, v);
    return sqrt(vget_lane_f32(v, 0));
#else
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1]);
#endif
}

EL_INLINE ELFloat ELVector2Distance(ELVector2 vectorStart, ELVector2 vectorEnd)
{
    return ELVector2Length(ELVector2Subtract(vectorEnd, vectorStart));
}

EL_INLINE ELVector2 ELVector2Lerp(ELVector2 vectorStart, ELVector2 vectorEnd, ELFloat t)
{
#if defined(__ARM_NEON__TODO)
    float32x2_t vDiff = vsub_f32(*(float32x2_t *)&vectorEnd,
                                 *(float32x2_t *)&vectorStart);
    vDiff = vmul_f32(vDiff, vdup_n_f32((ELFloat32_t)t));
    float32x2_t v = vadd_f32(*(float32x2_t *)&vectorStart, vDiff);
    return *(ELVector2 *)&v;
#else
    ELVector2 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
                    vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t) };
    return v;
#endif
}

EL_INLINE ELVector2 ELVector2Project(ELVector2 vectorToProject, ELVector2 projectionVector)
{
    ELFloat scale = ELVector2DotProduct(projectionVector, vectorToProject) / ELVector2DotProduct(projectionVector, projectionVector);
    ELVector2 v = ELVector2MultiplyScalar(projectionVector, scale);
    return v;
}


#endif //EZGL_ELVECTOR2_H
