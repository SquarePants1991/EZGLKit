//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_ELVECTOR3_H
#define EZGL_ELVECTOR3_H

#include "EZGLBasicTypes.h"
#include "EZGLVectorTypes.h"

EL_INLINE ELVector3 ELVector3Make(ELFloat x, ELFloat y, ELFloat z);
EL_INLINE ELVector3 ELVector3MakeWithArray(ELFloat values[3]);

EL_INLINE ELVector3 ELVector3Negate(ELVector3 vector);

EL_INLINE ELVector3 ELVector3Add(ELVector3 vectorLeft, ELVector3 vectorRight);
EL_INLINE ELVector3 ELVector3Subtract(ELVector3 vectorLeft, ELVector3 vectorRight);
EL_INLINE ELVector3 ELVector3Multiply(ELVector3 vectorLeft, ELVector3 vectorRight);
EL_INLINE ELVector3 ELVector3Divide(ELVector3 vectorLeft, ELVector3 vectorRight);

EL_INLINE ELVector3 ELVector3AddScalar(ELVector3 vector, ELFloat value);
EL_INLINE ELVector3 ELVector3SubtractScalar(ELVector3 vector, ELFloat value);
EL_INLINE ELVector3 ELVector3MultiplyScalar(ELVector3 vector, ELFloat value);
EL_INLINE ELVector3 ELVector3DivideScalar(ELVector3 vector, ELFloat value);

/*
 Returns a vector whose elements are the larger of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector3 ELVector3Maximum(ELVector3 vectorLeft, ELVector3 vectorRight);
/*
 Returns a vector whose elements are the smaller of the corresponding elements of the vector arguments.
 */
EL_INLINE ELVector3 ELVector3Minimum(ELVector3 vectorLeft, ELVector3 vectorRight);

/*
 Returns true if all of the first vector's elements are equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector3AllEqualToVector3(ELVector3 vectorLeft, ELVector3 vectorRight);
/*
 Returns true if all of the vector's elements are equal to the provided value.
 */
EL_INLINE bool ELVector3AllEqualToScalar(ELVector3 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than all of the second vector's arguments.
 */
EL_INLINE bool ELVector3AllGreaterThanVector3(ELVector3 vectorLeft, ELVector3 vectorRight);
/*
 Returns true if all of the vector's elements are greater than the provided value.
 */
EL_INLINE bool ELVector3AllGreaterThanScalar(ELVector3 vector, ELFloat value);
/*
 Returns true if all of the first vector's elements are greater than or equal to all of the second vector's arguments.
 */
EL_INLINE bool ELVector3AllGreaterThanOrEqualToVector3(ELVector3 vectorLeft, ELVector3 vectorRight);
/*
 Returns true if all of the vector's elements are greater than or equal to the provided value.
 */
EL_INLINE bool ELVector3AllGreaterThanOrEqualToScalar(ELVector3 vector, ELFloat value);

EL_INLINE ELVector3 ELVector3Normalize(ELVector3 vector);

EL_INLINE ELFloat ELVector3DotProduct(ELVector3 vectorLeft, ELVector3 vectorRight);
EL_INLINE ELFloat ELVector3Length(ELVector3 vector);
EL_INLINE ELFloat ELVector3Distance(ELVector3 vectorStart, ELVector3 vectorEnd);

EL_INLINE ELVector3 ELVector3Lerp(ELVector3 vectorStart, ELVector3 vectorEnd, ELFloat t);

EL_INLINE ELVector3 ELVector3CrossProduct(ELVector3 vectorLeft, ELVector3 vectorRight);

/*
 Project the vector, vectorToProject, onto the vector, projectionVector.
 */
EL_INLINE ELVector3 ELVector3Project(ELVector3 vectorToProject, ELVector3 projectionVector);



EL_INLINE ELVector3 ELVector3Make(ELFloat x, ELFloat y, ELFloat z)
{
    ELVector3 v = { x, y, z };
    return v;
}

EL_INLINE ELVector3 ELVector3MakeWithArray(ELFloat values[3])
{
    ELVector3 v = { values[0], values[1], values[2] };
    return v;
}

EL_INLINE ELVector3 ELVector3Negate(ELVector3 vector)
{
ELVector3 v = { -vector.v[0], -vector.v[1], -vector.v[2] };
return v;
}

EL_INLINE ELVector3 ELVector3Add(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 v = { vectorLeft.v[0] + vectorRight.v[0],
                 vectorLeft.v[1] + vectorRight.v[1],
                 vectorLeft.v[2] + vectorRight.v[2] };
return v;
}

EL_INLINE ELVector3 ELVector3Subtract(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 v = { vectorLeft.v[0] - vectorRight.v[0],
                 vectorLeft.v[1] - vectorRight.v[1],
                 vectorLeft.v[2] - vectorRight.v[2] };
return v;
}

EL_INLINE ELVector3 ELVector3Multiply(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 v = { vectorLeft.v[0] * vectorRight.v[0],
                 vectorLeft.v[1] * vectorRight.v[1],
                 vectorLeft.v[2] * vectorRight.v[2] };
return v;
}

EL_INLINE ELVector3 ELVector3Divide(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 v = { vectorLeft.v[0] / vectorRight.v[0],
                 vectorLeft.v[1] / vectorRight.v[1],
                 vectorLeft.v[2] / vectorRight.v[2] };
return v;
}

EL_INLINE ELVector3 ELVector3AddScalar(ELVector3 vector, ELFloat value)
{
ELVector3 v = { vector.v[0] + value,
                 vector.v[1] + value,
                 vector.v[2] + value };
return v;
}

EL_INLINE ELVector3 ELVector3SubtractScalar(ELVector3 vector, ELFloat value)
{
ELVector3 v = { vector.v[0] - value,
                 vector.v[1] - value,
                 vector.v[2] - value };
return v;
}

EL_INLINE ELVector3 ELVector3MultiplyScalar(ELVector3 vector, ELFloat value)
{
ELVector3 v = { vector.v[0] * value,
                 vector.v[1] * value,
                 vector.v[2] * value };
return v;
}

EL_INLINE ELVector3 ELVector3DivideScalar(ELVector3 vector, ELFloat value)
{
ELVector3 v = { vector.v[0] / value,
                 vector.v[1] / value,
                 vector.v[2] / value };
return v;
}

EL_INLINE ELVector3 ELVector3Maximum(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 max = vectorLeft;
if (vectorRight.v[0] > vectorLeft.v[0])
max.v[0] = vectorRight.v[0];
if (vectorRight.v[1] > vectorLeft.v[1])
max.v[1] = vectorRight.v[1];
if (vectorRight.v[2] > vectorLeft.v[2])
max.v[2] = vectorRight.v[2];
return max;
}

EL_INLINE ELVector3 ELVector3Minimum(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 min = vectorLeft;
if (vectorRight.v[0] < vectorLeft.v[0])
min.v[0] = vectorRight.v[0];
if (vectorRight.v[1] < vectorLeft.v[1])
min.v[1] = vectorRight.v[1];
if (vectorRight.v[2] < vectorLeft.v[2])
min.v[2] = vectorRight.v[2];
return min;
}

EL_INLINE bool ELVector3AllEqualToVector3(ELVector3 vectorLeft, ELVector3 vectorRight)
{
bool compare = false;
if (vectorLeft.v[0] == vectorRight.v[0] &&
vectorLeft.v[1] == vectorRight.v[1] &&
vectorLeft.v[2] == vectorRight.v[2])
compare = true;
return compare;
}

EL_INLINE bool ELVector3AllEqualToScalar(ELVector3 vector, ELFloat value)
{
bool compare = false;
if (vector.v[0] == value &&
        vector.v[1] == value &&
        vector.v[2] == value)
compare = true;
return compare;
}

EL_INLINE bool ELVector3AllGreaterThanVector3(ELVector3 vectorLeft, ELVector3 vectorRight)
{
bool compare = false;
if (vectorLeft.v[0] > vectorRight.v[0] &&
vectorLeft.v[1] > vectorRight.v[1] &&
vectorLeft.v[2] > vectorRight.v[2])
compare = true;
return compare;
}

EL_INLINE bool ELVector3AllGreaterThanScalar(ELVector3 vector, ELFloat value)
{
bool compare = false;
if (vector.v[0] > value &&
        vector.v[1] > value &&
        vector.v[2] > value)
compare = true;
return compare;
}

EL_INLINE bool ELVector3AllGreaterThanOrEqualToVector3(ELVector3 vectorLeft, ELVector3 vectorRight)
{
bool compare = false;
if (vectorLeft.v[0] >= vectorRight.v[0] &&
vectorLeft.v[1] >= vectorRight.v[1] &&
vectorLeft.v[2] >= vectorRight.v[2])
compare = true;
return compare;
}

EL_INLINE bool ELVector3AllGreaterThanOrEqualToScalar(ELVector3 vector, ELFloat value)
{
bool compare = false;
if (vector.v[0] >= value &&
        vector.v[1] >= value &&
        vector.v[2] >= value)
compare = true;
return compare;
}

EL_INLINE ELVector3 ELVector3Normalize(ELVector3 vector)
{
ELFloat scale = 1.0f / ELVector3Length(vector);
ELVector3 v = { vector.v[0] * scale, vector.v[1] * scale, vector.v[2] * scale };
return v;
}

EL_INLINE ELFloat ELVector3DotProduct(ELVector3 vectorLeft, ELVector3 vectorRight)
{
return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1] + vectorLeft.v[2] * vectorRight.v[2];
}

EL_INLINE ELFloat ELVector3Length(ELVector3 vector)
{
return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
}

EL_INLINE ELFloat ELVector3Distance(ELVector3 vectorStart, ELVector3 vectorEnd)
{
return ELVector3Length(ELVector3Subtract(vectorEnd, vectorStart));
}

EL_INLINE ELVector3 ELVector3Lerp(ELVector3 vectorStart, ELVector3 vectorEnd, ELFloat t)
{
ELVector3 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
                 vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
                 vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t) };
return v;
}

EL_INLINE ELVector3 ELVector3CrossProduct(ELVector3 vectorLeft, ELVector3 vectorRight)
{
ELVector3 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
                 vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
                 vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0] };
return v;
}

EL_INLINE ELVector3 ELVector3Project(ELVector3 vectorToProject, ELVector3 projectionVector)
{
ELFloat scale = ELVector3DotProduct(projectionVector, vectorToProject) / ELVector3DotProduct(projectionVector, projectionVector);
ELVector3 v = ELVector3MultiplyScalar(projectionVector, scale);
return v;
}

#endif //EZGL_ELVECTOR3_H
