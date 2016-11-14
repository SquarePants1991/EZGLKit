//
//  EZGLPlaneGeometry.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/10.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLPlaneGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLPlaneGeometry ()

@property (assign, nonatomic) CGSize size;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLPlaneGeometry
- (instancetype)initWithSize:(CGSize)size {
    self = [super init];
    if (self) {
        self.size = size;
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (GLGeometryData)genGeometryData {
    self.buffer = [EZGLGeometryVertexBuffer new];
    EZGeometryRect3 rect = {
        {0.5f * self.size.width, 0.0f, -0.5f * self.size.height},
        {0.5f * self.size.width, 0.0f, 0.5f * self.size.height},
        {-0.5f * self.size.width, 0.0f, 0.5f * self.size.height},
        {-0.5f * self.size.width, 0.0f, -0.5f * self.size.height},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0}
    };
    [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
    
    GLfloat *vertex = (GLfloat *)[self.buffer data];
    
    GLGeometryData data;
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, [self.buffer rawLength], vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    data.vertexCount = [self.buffer rawLength] / sizeof(EZGLGeometryVertex);
    data.vertexStride = sizeof(EZGLGeometryVertex);
    data.supportIndiceVBO = NO;
    return data;
}

- (NSArray *)rigidBodys {
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsBox:GLKVector3Make(self.size.width / 2, 0, self.size.height / 2) mass:0 geometry:self];
    return @[body];
}

@end
