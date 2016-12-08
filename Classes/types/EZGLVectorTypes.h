//
// Created by wangyang on 16/11/22.
//

#ifndef EZGL_EZGLVECTOR_H
#define EZGL_EZGLVECTOR_H

#include "EZGLBasicTypes.h"

union _ELVector2 {
    ELFloat v[2];
    struct {
        ELFloat x;
        ELFloat y;
    };
    struct {
        ELFloat s;
        ELFloat t;
    };
};
typedef union _ELVector2 ELVector2;

union _ELVector3 {
    ELFloat v[3];
    struct {
        ELVector2 xy;
        ELFloat zVal;
    };
    struct {
        ELVector2 xz;
        ELFloat yVal;
    };
    struct {
        ELVector2 yz;
        ELFloat xVal;
    };
    struct {
        ELFloat x;
        ELFloat y;
        ELFloat z;
    };
    struct {
        ELFloat r;
        ELFloat g;
        ELFloat b;
    };
};
typedef union _ELVector3 ELVector3;

union _ELVector4 {
    ELFloat v[4];
    struct {
        ELFloat x;
        ELFloat y;
        ELFloat z;
        ELFloat w;
    };
    struct {
        ELFloat r;
        ELFloat g;
        ELFloat b;
        ELFloat a;
    };
    struct {
        ELVector3 rgb;
        ELFloat aVal;
    };
    struct {
        ELVector3 xyz;
        ELFloat zVal;
    };
};
typedef union _ELVector4 ELVector4;

union _ELQuaternion {
    ELFloat q[4];
    struct {
        ELVector3 v;
        ELFloat s;
    };
    struct {
        ELFloat x, y, z, w;
    };
    struct {
        ELVector4 v4;
    };
};
typedef union _ELQuaternion ELQuaternion;

#endif //EZGL_EZGLVECTOR_H
