//
//  EZGLGeometryVertexBuffer.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLGeometryVertexBuffer.h"

@interface EZGLGeometryVertexBuffer () {
    EZGLGeometryVertex *vertices;
}

@property (assign, nonatomic) EZInt bufferLen;
@property (assign, nonatomic) EZInt index;

@end

@implementation EZGLGeometryVertexBuffer

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.bufferLen = 32;
        self.index = 0;
        vertices = (EZGLGeometryVertex *)malloc(sizeof(EZGLGeometryVertex) * self.bufferLen);
    }
    return self;
}

- (void)append:(EZGLGeometryVertex)vertex {
    *(vertices + self.index) = vertex;
    self.index++;
    if (self.index > self.bufferLen * 3 / 4.0) {
        self.bufferLen += 32;
        vertices = (EZGLGeometryVertex *)realloc(vertices, sizeof(EZGLGeometryVertex) * self.bufferLen);
    }
}

- (GLsizei)rawLength {
    return self.index * sizeof(EZGLGeometryVertex);
}

- (void *)data {
    return (void *)vertices;
}

- (void)clear {
    self.index = 0;
}

- (void)dealloc {
    free(vertices);
}

@end
