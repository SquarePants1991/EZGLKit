//
//  EZGLDefines.h
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#ifndef EZGLDefines_h
#define EZGLDefines_h

#import <GLKit/GLKit.h>

typedef GLfloat EZFloat;
typedef NSUInteger EZUInt;
typedef NSInteger EZInt;


typedef GLKVector3 EZVector3;
typedef GLKVector2 EZVector2;

typedef struct {
    EZVector3 point1;
    EZVector3 point2;
    EZVector3 point3;
    EZVector3 point4;
    EZVector2 uv1;
    EZVector2 uv2;
    EZVector2 uv3;
    EZVector2 uv4;
}EZGeometryRect3;

typedef struct {
    EZVector3 point1;
    EZVector3 point2;
    EZVector3 point3;
    EZVector2 uv1;
    EZVector2 uv2;
    EZVector2 uv3;
}EZGeometryTriangle;

typedef struct {
    EZFloat x;
    EZFloat y;
    EZFloat z;
    EZFloat nx;
    EZFloat ny;
    EZFloat nz;
    EZFloat u;
    EZFloat v;
    EZFloat tnx;
    EZFloat tny;
    EZFloat tnz;
    EZFloat btnx;
    EZFloat btny;
    EZFloat btnz;
}EZGLGeometryVertex;


#endif /* EZGLDefines_h */
