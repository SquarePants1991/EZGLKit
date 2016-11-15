//
//  EZGLGeometryVertexBuffer.h
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLDefines.h"

@interface EZGLGeometryVertexBuffer : NSObject

- (void)append:(EZGLGeometryVertex)vertex;
- (GLsizei)rawLength;
- (void *)data;
- (void)clear;

@end
