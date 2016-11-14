//
//  EZGLSplineCylinderGeometry.m
//  Pods
//
//  Created by wang yang on 2016/11/14.
//
//

#import "EZGLSplineCylinderGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLSplineCylinderGeometry ()

@property (assign, nonatomic) CGFloat height;
@property (assign, nonatomic) CGFloat radius;
@property (assign, nonatomic) CGFloat segments;
@property (assign, nonatomic) CGFloat ring;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLSplineCylinderGeometry
- (instancetype)initWithHeight:(CGFloat)height radius:(CGFloat)radius segments:(CGFloat)segments ring:(CGFloat)ring {
    self = [super init];
    if (self) {
        self.height = height;
        self.ring = ring;
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
    CGFloat ringSegHeight = self.height / self.ring;
    for (int ringIndex = 0; ringIndex < self.ring; ringIndex++) {
        CGFloat baseY0 = ringIndex * ringSegHeight - self.height / 2;
        CGFloat baseY1 = (ringIndex + 1) * ringSegHeight - self.height / 2;
        CGFloat radius = (cos(ringIndex / 4.0) + 2);
        CGFloat radiusNext = (cos((ringIndex + 1) / 4.0) + 2);
        for (int i=0;i< self.segments ;i++) {
            CGFloat radian = i / self.segments * M_PI * 2;
            CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
            
            CGFloat xb0 = cos(radian) * radius;
            CGFloat zb0 = sin(radian) * radius;
            CGFloat xb1 = cos(radianNext) * radius;
            CGFloat zb1 = sin(radianNext) * radius;
            
            CGFloat xt0 = cos(radian) * radiusNext;
            CGFloat zt0 = sin(radian) * radiusNext;
            CGFloat xt1 = cos(radianNext) * radiusNext;
            CGFloat zt1 = sin(radianNext) * radiusNext;
            
            EZGeometryRect3 rect = {
                {xt0, baseY1, zt0},
                {xb0, baseY0, zb0},
                {xb1, baseY0, zb1},
                {xt1, baseY1, zt1},
                {radian / (M_PI * 2), 1 - (baseY1 + self.height / 2) / self.height},
                {radian / (M_PI * 2), 1 - (baseY0 + self.height / 2) / self.height},
                {radianNext / (M_PI * 2), 1 - (baseY0 + self.height / 2) / self.height},
                {radianNext / (M_PI * 2), 1 - (baseY1 + self.height / 2) / self.height}
            };
            [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
        }
    }
    
    
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
