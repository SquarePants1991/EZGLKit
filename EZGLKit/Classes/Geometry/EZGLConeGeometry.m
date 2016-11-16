//
//  EZGLConeGeometry.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLConeGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLConeGeometry()

@property (assign, nonatomic) CGFloat height;
@property (assign, nonatomic) CGFloat radius;
@property (assign, nonatomic) CGFloat segments;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLConeGeometry
- (instancetype)initWithRadius:(CGFloat)radius segments:(CGFloat)segments height:(CGFloat)height {
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
    [self genConeIntoBuffer:self.buffer];
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

- (void)genConeIntoBuffer:(EZGLGeometryVertexBuffer *)buffer {
    // 底部面
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

    // 锥部分
    CGFloat ringHeight = 0;
    CGFloat ringRadius = self.radius;
    CGFloat ringHeightNext = self.height;
    CGFloat ringRadiusNext = 0;
    
    for (int j = 0; j < self.segments ; j++) {
        int segmentIndex = j;
        CGFloat radian = segmentIndex / self.segments * M_PI * 2;
        CGFloat radianNext = (segmentIndex + 1) / self.segments * M_PI * 2;
        
        CGFloat x_r1_x0 = cos(radian) * ringRadius;
        CGFloat x_r1_y0 = ringHeight;
        CGFloat x_r1_z0 = sin(radian) * ringRadius;
        CGFloat x_r1_x1 = cos(radianNext) * ringRadius;
        CGFloat x_r1_y1 = ringHeight;
        CGFloat x_r1_z1 = sin(radianNext) * ringRadius;
        
        CGFloat x_r2_x0 = cos(radian) * ringRadiusNext;
        CGFloat x_r2_y0 = ringHeightNext;
        CGFloat x_r2_z0 = sin(radian) * ringRadiusNext;
        
        EZGeometryTriangle triangle = {
            {x_r2_x0, x_r2_y0 - self.height / 2, x_r2_z0},
            {x_r1_x0, x_r1_y0 - self.height / 2, x_r1_z0},
            {x_r1_x1, x_r1_y1 - self.height / 2, x_r1_z1},
            {0.5, 1.0 - (x_r2_y0 + self.radius) / (self.radius * 2)},
            {radian / (M_PI * 2.0), 1.0 - (x_r1_y0 + self.radius) / (self.radius * 2)},
            {radianNext / (M_PI * 2.0), 1.0 - (x_r1_y1 + self.radius) / (self.radius * 2)}
        };
        [EZGLGeometryUtil appendTriangle:triangle toVertices:self.buffer];
    }
}

- (NSArray *)rigidBodys {
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsCone:self.radius height:self.height mass:1 geometry:self];
    return @[body];
}
@end
