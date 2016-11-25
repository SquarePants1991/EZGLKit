//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGEOMETRYTYPES_H
#define EZGL_ELGEOMETRYTYPES_H

#include "EZGLBasicTypes.h"
#include "ELVector2.h"
#include "ELVector3.h"

typedef struct {
    ELVector3 point1;
    ELVector3 point2;
    ELVector3 point3;
    ELVector3 point4;
    ELVector2 uv1;
    ELVector2 uv2;
    ELVector2 uv3;
    ELVector2 uv4;
}ELGeometryRect;

typedef struct {
    ELVector3 point1;
    ELVector3 point2;
    ELVector3 point3;
    ELVector2 uv1;
    ELVector2 uv2;
    ELVector2 uv3;
}ELGeometryTriangle;

typedef struct {
    ELFloat x;
    ELFloat y;
    ELFloat z;
    ELFloat nx;
    ELFloat ny;
    ELFloat nz;
    ELFloat u;
    ELFloat v;
    ELFloat tnx;
    ELFloat tny;
    ELFloat tnz;
    ELFloat btnx;
    ELFloat btny;
    ELFloat btnz;
}ELGeometryVertex;

#endif //EZGL_ELGEOMETRYTYPES_H
