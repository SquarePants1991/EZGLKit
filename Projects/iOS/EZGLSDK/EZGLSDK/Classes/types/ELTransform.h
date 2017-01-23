//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELTRANSFORM_H
#define EZGL_ELTRANSFORM_H

#include "ELVector3.h"
#include "ELMatrix4.h"

struct _ELTransform {
    ELVector3 position;
    ELQuaternion quaternion;
    ELVector3 scale;
};
typedef _ELTransform ELTransform;

const ELTransform ELTransformIdentity = {
        {0, 0, 0},
        {0, 0, 0, 0},
        {1, 1, 1}
};

EL_INLINE ELMatrix4 ELMatrix4FromTransform(ELTransform *transform) {
    ELMatrix4 matrix = ELMatrix4Identity;
    matrix = ELMatrix4Translate(matrix,transform->position.x, transform->position.y, transform->position.z);
    matrix = ELMatrix4Multiply(matrix,ELMatrix4MakeWithQuaternion(transform->quaternion));
    matrix = ELMatrix4Scale(matrix, transform->scale.x, transform->scale.y, transform->scale.z);
    return matrix;
}

#endif //EZGL_ELTRANSFORM_H
