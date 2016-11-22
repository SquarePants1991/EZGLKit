//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_EZGLMATRIX_H
#define EZGL_EZGLMATRIX_H

#include "EZGLBasicTypes.h"

union _ELMatrix2 {
    ELFloat m[4];
    struct
    {
        ELFloat m00, m01;
        ELFloat m10, m11;
    };
};
typedef union _ELMatrix2 ELMatrix2;

union _ELMatrix3 {
    ELFloat m[9];
    struct
    {
        ELFloat m00, m01, m02;
        ELFloat m10, m11, m12;
        ELFloat m20, m21, m22;
    };
};
typedef union _ELMatrix3 ELMatrix3;

union _ELMatrix4 {
    ELFloat m[16];
    struct
    {
        ELFloat m00, m01, m02, m03;
        ELFloat m10, m11, m12, m13;
        ELFloat m20, m21, m22, m23;
        ELFloat m30, m31, m32, m33;
    };
};
typedef union _ELMatrix4 ELMatrix4;


#endif //EZGL_EZGLMATRIX_H
