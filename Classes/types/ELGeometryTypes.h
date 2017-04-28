//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGEOMETRYTYPES_H
#define EZGL_ELGEOMETRYTYPES_H

#include "EZGLBasicTypes.h"
#include "ELVector2.h"
#include "ELVector3.h"

struct _ELMaterial {
    ELVector4 ambient;
    ELVector4 diffuse;
    ELVector4 specular;
    ELFloat shininess;
    ELUint ambientMap;
    ELUint diffuseMap;
    ELUint normalMap;
    ELUint specularMap;
};
typedef _ELMaterial ELMaterial;

const ELMaterial ELMaterialDefault = {
        {1.0, 1.0, 1.0, 1.0},
        {0.0, 0.0, 0.0, 1.0},
        {0.0, 0.0, 0.0, 1.0},
        90.0,
        0,
        0,
        0,
        0
};

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
    ELGeometryRect geometryRect;
    ELVector4 color1;
    ELVector4 color2;
    ELVector4 color3;
    ELVector4 color4;
}ELGeometryColorRect;

typedef struct {
    ELVector3 point1;
    ELVector3 point2;
    ELVector3 point3;
    ELVector2 uv1;
    ELVector2 uv2;
    ELVector2 uv3;
    ELInt matID;
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
    ELFloat matID;
    ELFloat tnx;
    ELFloat tny;
    ELFloat tnz;
    ELFloat btnx;
    ELFloat btny;
    ELFloat btnz;
    ELFloat clusterCount;
    ELFloat clusterID0;
    ELFloat weight0;
    ELFloat clusterID1;
    ELFloat weight1;
    ELFloat clusterID2;
    ELFloat weight2;
    ELFloat clusterID3;
    ELFloat weight3;
}ELGeometryVertex;

typedef struct {
    ELFloat x;
    ELFloat y;
    ELFloat z;
    ELFloat nx;
    ELFloat ny;
    ELFloat nz;
    ELFloat u;
    ELFloat v;
    ELFloat matID;
    ELFloat tnx;
    ELFloat tny;
    ELFloat tnz;
    ELFloat btnx;
    ELFloat btny;
    ELFloat btnz;
    ELVector4 color;
}ELGeometryColorVertex;

typedef struct {
    ELFloat x;
    ELFloat y;
    ELFloat z;
    ELFloat nx;
    ELFloat ny;
    ELFloat nz;
    ELFloat u;
    ELFloat v;
    ELFloat matID;
    ELFloat tnx;
    ELFloat tny;
    ELFloat tnz;
    ELFloat btnx;
    ELFloat btny;
    ELFloat btnz;
    ELFloat clusterCount;
    ELFloat clusterID0;
    ELFloat weight0;
    ELFloat clusterID1;
    ELFloat weight1;
    ELFloat clusterID2;
    ELFloat weight2;
    ELFloat clusterID3;
    ELFloat weight3;
}ELGeometryClusterVertex;

#endif //EZGL_ELGEOMETRYTYPES_H
