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

@property (assign, nonatomic) int vbo;
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
        self.vbo = -1;
        self.spline = [[EZGLSpline alloc] initWithLength:height segments:ring];
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (void)commitChanges {
    [self setupWithData:[self genGeometryData]];
}

- (GLGeometryData)genGeometryData {
    if (self.buffer == nil) {
        self.buffer = [EZGLGeometryVertexBuffer new];
    } else {
        [self.buffer clear];
    }
    
    // 顶部原型面
    CGFloat topRadius = self.radius + [self.spline offsetAtSegment:0];
    for (int i=0;i< self.segments ;i++) {
        CGFloat radian = i / self.segments * M_PI * 2;
        CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
        
        CGFloat x0 = cos(radian) * topRadius;
        CGFloat z0 = sin(radian) * topRadius;
        
        CGFloat x1 = cos(radianNext) * topRadius;
        CGFloat z1 = sin(radianNext) * topRadius;
        
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
    CGFloat bottomRadius = self.radius + [self.spline offsetAtSegment:self.ring - 1];
    for (int i=0;i< self.segments ;i++) {
        CGFloat radian = i / self.segments * M_PI * 2;
        CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
        
        CGFloat x0 = cos(radian) * bottomRadius;
        CGFloat z0 = sin(radian) * bottomRadius;
        
        CGFloat x1 = cos(radianNext) * bottomRadius;
        CGFloat z1 = sin(radianNext) * bottomRadius;
        
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
        CGFloat radius = self.radius + [self.spline offsetAtSegment:ringIndex];
        CGFloat radiusNext = self.radius + [self.spline offsetAtSegment:ringIndex + 1];
        for (int i=0;i< self.segments ;i++) {
            CGFloat radian = i / self.segments * M_PI * 2;
            CGFloat radianNext = (i + 1) / self.segments * M_PI * 2;
            if (i == self.segments - 1) {
                radianNext = 0;
            }
            
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
    
    [self.buffer caculatePerVertexNormal];
    
    GLfloat *vertex = (GLfloat *)[self.buffer data];
    
    GLGeometryData data;
    if (self.vbo >= 0) {
        data.vertexVBO = self.vbo;
    } else {
        glGenBuffers(1, &data.vertexVBO);
        self.vbo = data.vertexVBO;
    }
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, [self.buffer rawLength], vertex, GL_DYNAMIC_DRAW);
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
