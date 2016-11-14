//
//  EZGLSphereGeometry.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLSphereGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLSphereGeometry()

@property (assign, nonatomic) CGFloat radius;
@property (assign, nonatomic) CGFloat segments;
@property (assign, nonatomic) CGFloat ring;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLSphereGeometry
- (instancetype)initWithRadius:(CGFloat)radius segments:(CGFloat)segments ring:(CGFloat)ring {
    self = [super init];
    if (self) {
        self.ring = ring;
        self.radius = radius;
        self.segments = segments;
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (GLGeometryData)genGeometryData {
    self.buffer = [EZGLGeometryVertexBuffer new];
    [self genHalfSphere:1 intoBuffer:self.buffer];
    [self genHalfSphere:-1 intoBuffer:self.buffer];
    
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

- (void)genHalfSphere:(int)step intoBuffer:(EZGLGeometryVertexBuffer *)buffer {
    // 上半圆
    for (int i = 0;fabs(i) < self.ring;i+=step) {
        CGFloat ringHeight = self.radius * i / self.ring;
        CGFloat ringRadius = sqrt(pow(self.radius, 2) - pow(ringHeight, 2));
        CGFloat ringHeightNext = self.radius * (i + step) / self.ring;
        CGFloat ringRadiusNext = sqrt(pow(self.radius, 2) - pow(ringHeightNext, 2));
        
        for (int j=0;j< self.segments ;j++) {
            int segmentIndex = step > 0 ? j : (self.segments - j);
            CGFloat radian = segmentIndex / self.segments * M_PI * 2;
            CGFloat radianNext = (segmentIndex + step) / self.segments * M_PI * 2;
            
            CGFloat x_r1_x0 = cos(radian) * ringRadius;
            CGFloat x_r1_y0 = ringHeight;
            CGFloat x_r1_z0 = sin(radian) * ringRadius;
            CGFloat x_r1_x1 = cos(radianNext) * ringRadius;
            CGFloat x_r1_y1 = ringHeight;
            CGFloat x_r1_z1 = sin(radianNext) * ringRadius;
            
            CGFloat x_r2_x0 = cos(radian) * ringRadiusNext;
            CGFloat x_r2_y0 = ringHeightNext;
            CGFloat x_r2_z0 = sin(radian) * ringRadiusNext;
            CGFloat x_r2_x1 = cos(radianNext) * ringRadiusNext;
            CGFloat x_r2_y1 = ringHeightNext;
            CGFloat x_r2_z1 = sin(radianNext) * ringRadiusNext;
            
            BOOL isTriangle = x_r2_x0 == x_r2_x1 && x_r2_z0 == x_r2_z1;
            
            if (isTriangle) {
                EZGeometryTriangle triangle = {
                    {x_r2_x0, x_r2_y0, x_r2_z0},
                    {x_r1_x0, x_r1_y0, x_r1_z0},
                    {x_r1_x1, x_r1_y1, x_r1_z1},
                    {0.5, 1.0 - (x_r2_y0 + self.radius) / (self.radius * 2)},
                    {radian / (M_PI * 2.0), 1.0 - (x_r1_y0 + self.radius) / (self.radius * 2)},
                    {radianNext / (M_PI * 2.0), 1.0 - (x_r1_y1 + self.radius) / (self.radius * 2)}
                };
                [EZGLGeometryUtil appendTriangle:triangle toVertices:self.buffer];
            } else {
                EZGeometryRect3 rect = {
                    {x_r2_x0, x_r2_y0, x_r2_z0},
                    {x_r1_x0, x_r1_y0, x_r1_z0},
                    {x_r1_x1, x_r1_y1, x_r1_z1},
                    {x_r2_x1, x_r2_y1, x_r2_z1},
                    {radian / M_PI / 2, 1.0 - (x_r2_y0 + self.radius) / (self.radius * 2)},
                    {radian / M_PI / 2, 1.0 - (x_r1_y0 + self.radius) / (self.radius * 2)},
                    {radianNext / M_PI / 2, 1.0 - (x_r1_y1 + self.radius) / (self.radius * 2)},
                    {radianNext / M_PI / 2, 1.0 - (x_r2_y1 + self.radius) / (self.radius * 2)}
                };
                [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
            }
        }
    }
}

- (NSArray *)rigidBodys {
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsSphere:self.radius mass:1 geometry:self];
    return @[body];
}

@end
