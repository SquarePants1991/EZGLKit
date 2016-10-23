//
//  EZGLCubeGeometry.m
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import "EZGLCubeGeometry.h"

@implementation EZGLCubeGeometry

- (instancetype)init {
    self = [super init];
    if (self) {
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (GLGeometryData)genGeometryData {
    GLGeometryData data;
    size_t size = 10;
    const GLfloat vertex[4][9] = {
        { 0.5f * size, -0.5f * size, -0.5f * size,  0.0f, 0.0f, 1.0f,  0, 0 },
        { 0.5f * size, -0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  0, 1 },
        { -0.5f * size, -0.5f, 0.5f * size,  0.0f, 0.0f, 1.0f,   1, 1 },
        { 0.5f * size, 0.0f, -0.5f * size,  0.0f, 0.0f, 1.0f,  1, 0 }
    };
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    const GLuint indice[2][3] = {
        0, 1, 2,
        2, 3, 0
    };
    glGenBuffers(1, &data.indiceVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indiceVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    data.indiceCount = sizeof(indice) / sizeof(GLuint);
    data.vertexStride = sizeof(vertex[0]);
    data.supportIndiceVBO = YES;
    return data;
}

- (NSArray *)rigidBodys {
    EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsStaticPlane:100 geometry:self];
    return @[body];
}

@end
