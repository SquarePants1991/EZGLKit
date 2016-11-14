//
//  EZGLGeometryUtil.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLGeometryUtil.h"
#import <GLKit/GLKit.h>

@implementation EZGLGeometryUtil

+ (void)appendRect:(EZGeometryRect3)rect toVertices:(EZGLGeometryVertexBuffer *)vertices {
    EZVector3 point1 = rect.point1;
    EZVector3 point2 = rect.point2;
    EZVector3 point3 = rect.point3;
    EZVector3 point4 = rect.point4;
    
    
    GLKVector3 edge1 = GLKVector3Subtract((GLKVector3)point1, (GLKVector3)point2);
    GLKVector3 edge2 = GLKVector3Subtract((GLKVector3)point4, (GLKVector3)point2);
    GLKVector3 normal = GLKVector3CrossProduct(edge1, edge2);
    normal = GLKVector3Normalize(normal);
    
    EZGLGeometryVertex vertex1_1 = {point1.x, point1.y, point1.z, normal.x, normal.y, normal.z, rect.uv1.x, rect.uv1.y};
    EZGLGeometryVertex vertex1_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    EZGLGeometryVertex vertex1_3 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};
    
    GLKVector3 tangent1_1,bitangent1_1;
    GLKVector3 tangent1_2,bitangent1_2;
    GLKVector3 tangent1_3,bitangent1_3;
    [self caculateTangents:&tangent1_1 bitangents:&bitangent1_1 position:(GLKVector3)point1 normal:normal];
    [self caculateTangents:&tangent1_2 bitangents:&bitangent1_2 position:(GLKVector3)point2 normal:normal];
    [self caculateTangents:&tangent1_3 bitangents:&bitangent1_3 position:(GLKVector3)point3 normal:normal];
    vertex1_1.tnx = tangent1_1.x;       vertex1_1.tny = tangent1_1.y;       vertex1_1.tnz = tangent1_1.z;
    vertex1_1.btnx = bitangent1_1.x;    vertex1_1.btny = bitangent1_1.y;    vertex1_1.btnz = bitangent1_1.z;
    vertex1_2.tnx = tangent1_2.x;       vertex1_2.tny = tangent1_2.y;       vertex1_2.tnz = tangent1_2.z;
    vertex1_2.btnx = bitangent1_2.x;    vertex1_2.btny = bitangent1_2.y;    vertex1_2.btnz = bitangent1_2.z;
    vertex1_3.tnx = tangent1_3.x;       vertex1_3.tny = tangent1_3.y;       vertex1_3.tnz = tangent1_3.z;
    vertex1_3.btnx = bitangent1_3.x;    vertex1_3.btny = bitangent1_3.y;    vertex1_3.btnz = bitangent1_3.z;
    
    
    [vertices append:vertex1_1];
    [vertices append:vertex1_2];
    [vertices append:vertex1_3];
    
    
    
    edge1 = GLKVector3Subtract((GLKVector3)point4, (GLKVector3)point2);
    edge2 = GLKVector3Subtract((GLKVector3)point3, (GLKVector3)point2);
    normal = GLKVector3CrossProduct(edge1, edge2);
    normal = GLKVector3Normalize(normal);
    
    EZGLGeometryVertex vertex2_1 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};
    EZGLGeometryVertex vertex2_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    EZGLGeometryVertex vertex2_3 = {point3.x, point3.y, point3.z, normal.x, normal.y, normal.z, rect.uv3.x, rect.uv3.y};
    
    GLKVector3 tangent2_1,bitangent2_1;
    GLKVector3 tangent2_2,bitangent2_2;
    GLKVector3 tangent2_3,bitangent2_3;
    [self caculateTangents:&tangent2_1 bitangents:&bitangent2_1 position:(GLKVector3)point1 normal:normal];
    [self caculateTangents:&tangent2_2 bitangents:&bitangent2_2 position:(GLKVector3)point4 normal:normal];
    [self caculateTangents:&tangent2_3 bitangents:&bitangent2_3 position:(GLKVector3)point3 normal:normal];
    vertex2_1.tnx = tangent2_1.x;       vertex2_1.tny = tangent2_1.y;       vertex2_1.tnz = tangent2_1.z;
    vertex2_1.btnx = bitangent2_1.x;    vertex2_1.btny = bitangent2_1.y;    vertex2_1.btnz = bitangent2_1.z;
    vertex2_2.tnx = tangent2_2.x;       vertex2_2.tny = tangent2_2.y;       vertex2_2.tnz = tangent2_2.z;
    vertex2_2.btnx = bitangent2_2.x;    vertex2_2.btny = bitangent2_2.y;    vertex2_2.btnz = bitangent2_2.z;
    vertex2_3.tnx = tangent2_3.x;       vertex2_3.tny = tangent2_3.y;       vertex2_3.tnz = tangent2_3.z;
    vertex2_3.btnx = bitangent2_3.x;    vertex2_3.btny = bitangent2_3.y;    vertex2_3.btnz = bitangent2_3.z;
    
    [vertices append:vertex2_1];
    [vertices append:vertex2_2];
    [vertices append:vertex2_3];
}

+ (void)appendTriangle:(EZGeometryTriangle)triangle toVertices:(EZGLGeometryVertexBuffer *)vertices {
    EZVector3 point1 = triangle.point1;
    EZVector3 point2 = triangle.point2;
    EZVector3 point3 = triangle.point3;
    
    
    GLKVector3 edge1 = GLKVector3Subtract((GLKVector3)point3, (GLKVector3)point1);
    GLKVector3 edge2 = GLKVector3Subtract((GLKVector3)point2, (GLKVector3)point1);
    GLKVector3 normal = GLKVector3CrossProduct(edge1, edge2);
    normal = GLKVector3Normalize(normal);
    
    EZGLGeometryVertex vertex1_1 = {point1.x, point1.y, point1.z, normal.x, normal.y, normal.z, triangle.uv1.x, triangle.uv1.y};
    EZGLGeometryVertex vertex1_2 = {point3.x, point3.y, point3.z, normal.x, normal.y, normal.z, triangle.uv3.x, triangle.uv3.y};
    EZGLGeometryVertex vertex1_3 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, triangle.uv2.x, triangle.uv2.y};
    
    GLKVector3 tangent1_1,bitangent1_1;
    GLKVector3 tangent1_2,bitangent1_2;
    GLKVector3 tangent1_3,bitangent1_3;
    [self caculateTangents:&tangent1_1 bitangents:&bitangent1_1 position:(GLKVector3)point1 normal:normal];
    [self caculateTangents:&tangent1_2 bitangents:&bitangent1_2 position:(GLKVector3)point2 normal:normal];
    [self caculateTangents:&tangent1_3 bitangents:&bitangent1_3 position:(GLKVector3)point3 normal:normal];
    vertex1_1.tnx = tangent1_1.x;       vertex1_1.tny = tangent1_1.y;       vertex1_1.tnz = tangent1_1.z;
    vertex1_1.btnx = bitangent1_1.x;    vertex1_1.btny = bitangent1_1.y;    vertex1_1.btnz = bitangent1_1.z;
    vertex1_2.tnx = tangent1_2.x;       vertex1_2.tny = tangent1_2.y;       vertex1_2.tnz = tangent1_2.z;
    vertex1_2.btnx = bitangent1_2.x;    vertex1_2.btny = bitangent1_2.y;    vertex1_2.btnz = bitangent1_2.z;
    vertex1_3.tnx = tangent1_3.x;       vertex1_3.tny = tangent1_3.y;       vertex1_3.tnz = tangent1_3.z;
    vertex1_3.btnx = bitangent1_3.x;    vertex1_3.btny = bitangent1_3.y;    vertex1_3.btnz = bitangent1_3.z;
    
    
    [vertices append:vertex1_1];
    [vertices append:vertex1_2];
    [vertices append:vertex1_3];
}

+ (void)caculateTangents:(GLKVector3 *)pTangent bitangents:(GLKVector3 *)pBitangent position:(GLKVector3)position normal:(GLKVector3)normal {
    CGFloat x = position.x;
    CGFloat y = position.y;
    CGFloat z = position.z;
    CGFloat nx = normal.x;
    CGFloat ny = normal.y;
    CGFloat nz = normal.z;
    
    CGFloat x1 = x + 1;CGFloat z1 = z + 1;CGFloat y1 = ny == 0 ? y : (nz * (z - z1) + nx * (x - x1))/ny + y;
    GLKVector3 resultY = GLKVector3Make(x1 - x, y1 - y, z1 - z);
    
    x1 = x + 1;y1 = y + 1;z1 = nz == 0 ? z : (ny * (y - y1) + nx * (x - x1))/nz + z;
    GLKVector3 resultZ = GLKVector3Make(x1 - x, y1 - y, z1 - z);
    
    z1 = z + 1;y1 = y + 1;x1 = nx == 0 ? x : (ny * (y - y1) + nz * (z - z1))/nx + x;
    GLKVector3 resultX = GLKVector3Make(x1 - x, y1 - y, z1 - z);
    
    if (resultX.x < 50) {
        *pTangent = resultX;
        *pBitangent = GLKVector3CrossProduct(resultX,normal);
    } else if (resultY.y < 50) {
        *pTangent = resultY;
        *pBitangent = GLKVector3CrossProduct(resultY,normal);
    } else {
        *pTangent = resultZ;
        *pBitangent = GLKVector3CrossProduct(resultZ,normal);
    }
}

@end
