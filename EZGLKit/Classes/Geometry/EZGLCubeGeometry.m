//
//  EZGLCubeGeometry.m
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import "EZGLCubeGeometry.h"
#import "EZGLGeometryUtil.h"

@interface EZGLCubeGeometry ()

@property (assign, nonatomic) GLKVector3 size;
@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLCubeGeometry

- (instancetype)initWithSize:(GLKVector3)size {
    self = [super init];
    if (self) {
        self.size = size;
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (GLGeometryData)genGeometryData {
    GLGeometryData data;
    self.buffer = [EZGLGeometryVertexBuffer new];
    // 柱子
    int segments = 4;
    
    // x 轴上的面
    EZGeometryRect3 rectXLeft = {
        {-self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {-self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    
    EZGeometryRect3 rectXRight = {
        {self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    [EZGLGeometryUtil appendRect:rectXLeft toVertices:self.buffer];
    [EZGLGeometryUtil appendRect:rectXRight toVertices:self.buffer];
    
    // y 轴上的面
    EZGeometryRect3 rectYDown = {
        {-self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {-self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    
    EZGeometryRect3 rectYUp = {
        {self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    [EZGLGeometryUtil appendRect:rectYUp toVertices:self.buffer];
    [EZGLGeometryUtil appendRect:rectYDown toVertices:self.buffer];

    // z 轴上的面
    EZGeometryRect3 rectZFar = {
        {-self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {-self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, -self.size.z / 2},
        {self.size.x / 2, self.size.y / 2, -self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    
    EZGeometryRect3 rectZNear = {
        {self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, -self.size.y / 2, self.size.z / 2},
        {-self.size.x / 2, self.size.y / 2, self.size.z / 2},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 0},
    };
    [EZGLGeometryUtil appendRect:rectZFar toVertices:self.buffer];
    [EZGLGeometryUtil appendRect:rectZNear toVertices:self.buffer];

    
    GLfloat *vertex = (GLfloat *)[self.buffer data];
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
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsBox: GLKVector3MultiplyScalar(self.size, 0.5) mass:1 geometry:self];
    return @[body];
}

@end
