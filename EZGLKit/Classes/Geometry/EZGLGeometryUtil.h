//
//  EZGLGeometryUtil.h
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLDefines.h"
#import "EZGLGeometryVertexBuffer.h"

@interface EZGLGeometryUtil : NSObject

+ (void)appendRect:(EZGeometryRect3)rect toVertices:(EZGLGeometryVertexBuffer *)vertices;
+ (void)appendTriangle:(EZGeometryTriangle)triangle toVertices:(EZGLGeometryVertexBuffer *)vertices;

@end
