//
//  ELFrustumTest.cpp
//  EZGLSDK
//
//  Created by wangyang on 2017/5/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#include "ELFrustumTest.h"
enum planeEnum
{
    NEAR_PLANE = 0,
    FAR_PLANE,
    LEFT_PLANE,
    RIGHT_PLANE,
    TOP_PLANE,
    BOTTOM_PLANE,
    PLANE_NUM
};

typedef struct {
    ELVector3 normal;
    ELFloat d;
}Plane;

// calculate the view frustum from the view-projection matrix
// (projection-matrix -> view-space; view-projection-matrix -> world-space etc...)
void createViewFrustumFromMatrix(ELMatrix4 mat, Plane *planes)
{
    // left
    planes[LEFT_PLANE].normal.x = mat.m03 + mat.m00;
    planes[LEFT_PLANE].normal.y = mat.m13 + mat.m10;
    planes[LEFT_PLANE].normal.z = mat.m23 + mat.m20;
    planes[LEFT_PLANE].d        = mat.m33 + mat.m30;
    
    // right
    planes[RIGHT_PLANE].normal.x = mat.m03 - mat.m00;
    planes[RIGHT_PLANE].normal.y = mat.m13 - mat.m10;
    planes[RIGHT_PLANE].normal.z = mat.m23 - mat.m20;
    planes[RIGHT_PLANE].d        = mat.m33 - mat.m30;
    
    // bottom
    planes[BOTTOM_PLANE].normal.x = mat.m03 + mat.m01;
    planes[BOTTOM_PLANE].normal.y = mat.m13 + mat.m11;
    planes[BOTTOM_PLANE].normal.z = mat.m23 + mat.m21;
    planes[BOTTOM_PLANE].d        = mat.m33 + mat.m31;
    
    // top
    planes[TOP_PLANE].normal.x = mat.m03 - mat.m01;
    planes[TOP_PLANE].normal.y = mat.m13 - mat.m11;
    planes[TOP_PLANE].normal.z = mat.m23 - mat.m21;
    planes[TOP_PLANE].d        = mat.m33 - mat.m31;
    
    // near
    planes[NEAR_PLANE].normal.x = mat.m03 + mat.m02;
    planes[NEAR_PLANE].normal.y = mat.m13 + mat.m12;
    planes[NEAR_PLANE].normal.z = mat.m23 + mat.m22;
    planes[NEAR_PLANE].d        = mat.m33 + mat.m32;
    
    // far
    planes[FAR_PLANE].normal.x = mat.m03 - mat.m02;
    planes[FAR_PLANE].normal.y = mat.m13 - mat.m12;
    planes[FAR_PLANE].normal.z = mat.m23 - mat.m22;
    planes[FAR_PLANE].d        = mat.m33 - mat.m32;
    
    // normalize
    for(int i = 0; i < PLANE_NUM; i++)
    {
        float length = ELVector3Length(planes[i].normal);
        planes[i].normal = ELVector3DivideScalar(planes[i].normal, length);
        planes[i].d /= length;
    }
}

float distanceFromPlaneToPoint(Plane plane,ELVector3 pt)
{
    return ELVector3DotProduct(plane.normal, pt) + plane.d;
}

bool sphereIsInFrustum(ELVector3 center, ELFloat radius, Plane *planes)
{
    for(int i = 0; i < PLANE_NUM; i++)
    {
        ELFloat distance = distanceFromPlaneToPoint(planes[i], center);
        if(distance < -radius)
        {
            return false;
        }
    }
    return true;
}

bool isInViewFrustum(ELMatrix4 frustumMatrix, ELVector3 size, ELMatrix4 modelMatrix) {
    Plane planes[PLANE_NUM];
    createViewFrustumFromMatrix(frustumMatrix, planes);
    ELFloat radius = ELVector3Length(size) / 2.0;
    ELVector4 center = ELVector4Make(0, 0, 0, 1);
    center = ELMatrix4MultiplyVector4(modelMatrix, center);
    return sphereIsInFrustum(center.xyz, radius, planes);
}
