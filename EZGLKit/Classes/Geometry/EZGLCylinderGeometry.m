//
//  EZGLCylinderGeometry.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLCylinderGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLCylinderGeometry ()

@property (assign, nonatomic) CGFloat height;
@property (assign, nonatomic) CGFloat radius;
@property (assign, nonatomic) CGFloat segments;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLCylinderGeometry
- (instancetype)initWithHeight:(CGFloat)height radius:(CGFloat)radius segments:(CGFloat)segments {
    self = [super init];
    if (self) {
        self.height = height;
        self.radius = radius;
        self.segments = segments;
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (GLGeometryData)genGeometryData {
    self.buffer = [EZGLGeometryVertexBuffer new];
    
    // 顶部原型面
    for (int i=0;i< self.segments ;i++) {
        CGFloat radian = i / self.segments * M_PI * 2;
        CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
        if (i == self.segments - 1) {
            radianNext = 0;
        }
        
        CGFloat x0 = cos(radian) * self.radius;
        CGFloat z0 = sin(radian) * self.radius;
        
        CGFloat x1 = cos(radianNext) * self.radius;
        CGFloat z1 = sin(radianNext) * self.radius;
        
        EZGeometryTriangle triangle = {
            {0, self.height / 2, 0},
            {x0, self.height / 2, z0},
            {x1, self.height / 2, z1},
            {0.5, 0.5},
            {cos(radian) * 0.5 + 0.5, sin(radian) * 0.5 + 0.5},
            {cos(radianNext) * 0.5 + 0.5, sin(radianNext) * 0.5 + 0.5}
        };
        [EZGLGeometryUtil appendTriangle:triangle toVertices:self.buffer];
    }

    // 底部原型面
    for (int i=0;i< self.segments ;i++) {
        CGFloat radian = i / self.segments * M_PI * 2;
        CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
        if (i == self.segments - 1) {
            radianNext = 0;
        }
        
        CGFloat x0 = cos(radian) * self.radius;
        CGFloat z0 = sin(radian) * self.radius;
        
        CGFloat x1 = cos(radianNext) * self.radius;
        CGFloat z1 = sin(radianNext) * self.radius;
        
        EZGeometryTriangle triangle = {
            {0, -self.height / 2, 0},
            {x1, -self.height / 2, z1},
            {x0, -self.height / 2, z0},
            {0.5, 0.5},
            {cos(radianNext) * 0.5 + 0.5, sin(radianNext) * 0.5 + 0.5},
            {cos(radian) * 0.5 + 0.5, sin(radian) * 0.5 + 0.5}
        };
        [EZGLGeometryUtil appendTriangle:triangle toVertices:self.buffer];
    }

    
    // 柱子
    for (int i=0;i < self.segments ;i++) {
        CGFloat radian = i / self.segments * M_PI * 2;
        CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
        if (i == self.segments - 1) {
            radianNext = 0;
        }
        
        CGFloat x0 = cos(radian) * self.radius;
        CGFloat z0 = sin(radian) * self.radius;
        
        CGFloat x1 = cos(radianNext) * self.radius;
        CGFloat z1 = sin(radianNext) * self.radius;
        
        EZGeometryRect3 rect = {
            {x0, self.height / 2, z0},
            {x0, -self.height / 2, z0},
            {x1, -self.height / 2, z1},
            {x1, self.height / 2, z1},
            {radian / (M_PI * 2), 0},
            {radian / (M_PI * 2), 1},
            {radianNext / (M_PI * 2), 1},
            {radianNext / (M_PI * 2), 0}
        };
        [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
    }
    
    [self.buffer caculatePerVertexNormal];
    
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
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsBox:GLKVector3Make(self.radius, self.height / 2 , self.radius) mass:1 geometry:self];
    return @[body];
}

@end

