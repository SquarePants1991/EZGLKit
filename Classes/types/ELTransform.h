//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELTRANSFORM_H
#define EZGL_ELTRANSFORM_H

#include "ELVector3.h"

struct _ELTransform {
    ELVector3 position;
    ELQuaternion quaternion;
    ELVector3 scale;
};
typedef _ELTransform ELTransform;

#endif //EZGL_ELTRANSFORM_H
