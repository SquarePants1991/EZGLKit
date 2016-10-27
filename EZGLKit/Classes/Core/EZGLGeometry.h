//
//  Geometry.h
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>

#import "EZGLDefines.h"
#import "EZGLProgram.h"
#import "EZGLMaterial.h"
#import "EZGLPhysicsWorld.h"
#import "EZGLRigidBody.h"
#import "EZGLTransform.h"

@class EZGLWorld;

typedef struct {
    GLuint vertexVBO;
    GLuint indiceVBO;
    GLsizei indiceCount;
    GLuint vertexStride;
    GLuint vertexCount;
    BOOL supportIndiceVBO;
}GLGeometryData;

@interface EZGLGeometry : NSObject
@property (strong, nonatomic) EZGLProgram *glProgram;
@property (weak, nonatomic) EZGLWorld *world;

@property (assign, nonatomic) GLKMatrix4 viewProjection;
@property (assign, nonatomic) GLKMatrix4 lightViewProjection;
@property (strong, nonatomic) EZGLMaterial *material;
@property (strong, nonatomic) EZGLTransform *transform;

@property (assign, nonatomic) BOOL renderAsShadow;

- (instancetype)initWithMaterial:(EZGLMaterial *)material;
- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader;
- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader material:(EZGLMaterial *)material;

- (void)setupWithData:(GLGeometryData)data;
- (void)prepare;
- (void)draw;
- (void)update:(NSTimeInterval)interval;
- (NSArray *)rigidBodys;
@end
