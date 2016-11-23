//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_ELQUATERNION_H
#define EZGL_ELQUATERNION_H

#include "EZGLVectorTypes.h"

#include "ELVector2.h"
#include "ELVector3.h"
#include "ELVector4.h"

EL_INLINE ELQuaternion ELQuaternionMake(ELFloat x, ELFloat y, ELFloat z, ELFloat w)
{
    ELQuaternion q = { x, y, z, w };
    return q;
}

EL_INLINE ELQuaternion ELQuaternionMakeWithVector3(ELVector3 vector, ELFloat scalar)
{
    ELQuaternion q = { vector.v[0], vector.v[1], vector.v[2], scalar };
    return q;
}

EL_INLINE ELQuaternion ELQuaternionMakeWithArray(ELFloat values[4])
{
#if defined(EL_SSE3_INTRINSICS)
    __m128 v = _mm_load_ps(values);
    return *(ELQuaternion *)&v;
#else
    ELQuaternion q = { values[0], values[1], values[2], values[3] };
    return q;
#endif
}

EL_INLINE ELQuaternion ELQuaternionMakeWithAngleAndAxis(ELFloat radians, ELFloat x, ELFloat y, ELFloat z)
{
    ELFloat halfAngle = radians * 0.5f;
    ELFloat scale = sinf(halfAngle);
    ELQuaternion q = { scale * x, scale * y, scale * z, cosf(halfAngle) };
    return q;
}

EL_INLINE ELQuaternion ELQuaternionMakeWithAngleAndVector3Axis(ELFloat radians, ELVector3 axisVector)
{
    return ELQuaternionMakeWithAngleAndAxis(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
}

EL_INLINE ELQuaternion ELQuaternionAdd(ELQuaternion quaternionLeft, ELQuaternion quaternionRight)
{
    ELQuaternion q = { quaternionLeft.q[0] + quaternionRight.q[0],
                       quaternionLeft.q[1] + quaternionRight.q[1],
                       quaternionLeft.q[2] + quaternionRight.q[2],
                       quaternionLeft.q[3] + quaternionRight.q[3] };
    return q;
}

EL_INLINE ELQuaternion ELQuaternionSubtract(ELQuaternion quaternionLeft, ELQuaternion quaternionRight)
{
    ELQuaternion q = { quaternionLeft.q[0] - quaternionRight.q[0],
                       quaternionLeft.q[1] - quaternionRight.q[1],
                       quaternionLeft.q[2] - quaternionRight.q[2],
                       quaternionLeft.q[3] - quaternionRight.q[3] };
    return q;
}

EL_INLINE ELQuaternion ELQuaternionMultiply(ELQuaternion quaternionLeft, ELQuaternion quaternionRight)
{
    ELQuaternion q = { quaternionLeft.q[3] * quaternionRight.q[0] +
                       quaternionLeft.q[0] * quaternionRight.q[3] +
                       quaternionLeft.q[1] * quaternionRight.q[2] -
                       quaternionLeft.q[2] * quaternionRight.q[1],

                       quaternionLeft.q[3] * quaternionRight.q[1] +
                       quaternionLeft.q[1] * quaternionRight.q[3] +
                       quaternionLeft.q[2] * quaternionRight.q[0] -
                       quaternionLeft.q[0] * quaternionRight.q[2],

                       quaternionLeft.q[3] * quaternionRight.q[2] +
                       quaternionLeft.q[2] * quaternionRight.q[3] +
                       quaternionLeft.q[0] * quaternionRight.q[1] -
                       quaternionLeft.q[1] * quaternionRight.q[0],

                       quaternionLeft.q[3] * quaternionRight.q[3] -
                       quaternionLeft.q[0] * quaternionRight.q[0] -
                       quaternionLeft.q[1] * quaternionRight.q[1] -
                       quaternionLeft.q[2] * quaternionRight.q[2] };
    return q;
}

EL_INLINE ELFloat ELQuaternionLength(ELQuaternion quaternion)
{
    return sqrt(quaternion.q[0] * quaternion.q[0] +
                quaternion.q[1] * quaternion.q[1] +
                quaternion.q[2] * quaternion.q[2] +
                quaternion.q[3] * quaternion.q[3]);
}

EL_INLINE ELQuaternion ELQuaternionConjugate(ELQuaternion quaternion)
{
    ELQuaternion q = { -quaternion.q[0], -quaternion.q[1], -quaternion.q[2], quaternion.q[3] };
    return q;
}


EL_INLINE ELQuaternion ELQuaternionNormalize(ELQuaternion quaternion) {
    ELFloat* v = ELVector4Normalize(quaternion.v4).v;
    return ELQuaternionMake(v[0],v[1],v[2],v[3]);
}

EL_INLINE ELQuaternion ELQuaternionInvert(ELQuaternion quaternion)
{
    ELFloat scale = 1.0f / (quaternion.q[0] * quaternion.q[0] +
                              quaternion.q[1] * quaternion.q[1] +
                              quaternion.q[2] * quaternion.q[2] +
                              quaternion.q[3] * quaternion.q[3]);
    ELQuaternion q = { -quaternion.q[0] * scale, -quaternion.q[1] * scale, -quaternion.q[2] * scale, quaternion.q[3] * scale };
    return q;
}

EL_INLINE ELVector3 ELQuaternionRotateVector3(ELQuaternion quaternion, ELVector3 vector)
{
    ELQuaternion rotatedQuaternion = ELQuaternionMake(vector.v[0], vector.v[1], vector.v[2], 0.0f);
    rotatedQuaternion = ELQuaternionMultiply(ELQuaternionMultiply(quaternion, rotatedQuaternion), ELQuaternionInvert(quaternion));

    return ELVector3Make(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2]);
}

EL_INLINE ELVector4 ELQuaternionRotateVector4(ELQuaternion quaternion, ELVector4 vector)
{
    ELQuaternion rotatedQuaternion = ELQuaternionMake(vector.v[0], vector.v[1], vector.v[2], 0.0f);
    rotatedQuaternion = ELQuaternionMultiply(ELQuaternionMultiply(quaternion, rotatedQuaternion), ELQuaternionInvert(quaternion));

    return ELVector4Make(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2], vector.v[3]);
}


#endif //EZGL_ELQUATERNION_H
