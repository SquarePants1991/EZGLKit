//
//  Geometry.m
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLGeometry.h"
#import "UIImage+GL.h"
#import "EZGLWaveFrontFile.h"
#import "EZGLDefines.h"
#import "EZGLLight.h"
#import "EZGLWorld.h"

@interface EZGLGeometry () {
    GLfloat rotation;
    GLuint currentTexture;
    float elapsedTime;
}

@property (strong, nonatomic) NSArray *textures;
@property (assign, nonatomic) GLuint vao;
@property (assign, nonatomic) GLGeometryData data;

@property (copy, nonatomic) NSString *vertexShader;
@property (copy, nonatomic) NSString *fragmentShader;

@property (assign, nonatomic) GLKMatrix4 modelMatrix;
@property (assign, nonatomic) GLKMatrix3 normalMatrix;
@end

@implementation EZGLGeometry

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.material = [EZGLMaterial defaultMaterial];
        [self setupTransform];
    }
    return self;
}

- (instancetype)initWithMaterial:(EZGLMaterial *)material {
    self = [super init];
    if (self) {
        self.material = material;
    }
    return self;
}

- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader {
    self = [super init];
    if (self) {
        self.vertexShader = vertexShader;
        self.fragmentShader = fragmentShader;
        self.material = [EZGLMaterial defaultMaterial];
    }
    return self;
}

- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader material:(EZGLMaterial *)material {
    self = [super init];
    if (self) {
        self.vertexShader = vertexShader;
        self.fragmentShader = fragmentShader;
        self.material = material;
    }
    return self;
}

- (void)setupWithData:(GLGeometryData)data {
    self.data = data;
    if (self.vertexShader && self.fragmentShader) {
        self.glProgram = [[EZGLProgram alloc]initWithVertexShaderFileName:self.vertexShader fragmentShaderFileName:self.fragmentShader];
    }
}

- (void)prepare {
    [self createTexture];
    [self setupVAO];
}

- (void)setupVAO {
    glGenVertexArraysOES(1, &_vao);
    glBindVertexArrayOES(self.vao);

    glBindBuffer(GL_ARRAY_BUFFER, self.data.vertexVBO);

    GLuint positionLocation = glGetAttribLocation(self.glProgram.value, "position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, self.data.vertexStride, 0);

    GLuint normalLocation = glGetAttribLocation(self.glProgram.value, "normal");
    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, self.data.vertexStride, BUFFER_OFFSET(3 * sizeof(GLfloat)));

    GLuint uvLocation = glGetAttribLocation(self.glProgram.value, "texcoord");
    glEnableVertexAttribArray(uvLocation);
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, self.data.vertexStride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
    
    GLuint tangentLocation = glGetAttribLocation(self.glProgram.value, "tangent");
    glEnableVertexAttribArray(tangentLocation);
    glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, self.data.vertexStride, BUFFER_OFFSET(8 * sizeof(GLfloat)));
    
    GLuint bitangentLocation = glGetAttribLocation(self.glProgram.value, "bitangent");
    glEnableVertexAttribArray(bitangentLocation);
    glVertexAttribPointer(bitangentLocation, 3, GL_FLOAT, GL_FALSE, self.data.vertexStride, BUFFER_OFFSET(11 * sizeof(GLfloat)));

    if (self.data.indiceVBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.data.indiceVBO);
    }

    glBindVertexArrayOES(0);
}

- (void)setupTransform {
    self.transform = [EZGLTransform new];
}

- (void)createTexture {
    self.textures = [UIImage texturesFromGif:@"demo"];
    currentTexture = 0;
}

- (void)draw {
    glUseProgram(self.glProgram.value);

    [self.world.effect applyToProgram:self.glProgram];
    
    GLint renderAsShadow = self.renderAsShadow ? 1 : 0;
    glUniform1i([self.glProgram uniform:UNIFORM_RENDERASSHADOW], renderAsShadow);

    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_VIEWPROJECTION], 1, 0, self.viewProjection.m);
    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_MODEL_MATRIX], 1, 0, self.modelMatrix.m);
    glUniformMatrix3fv([self.glProgram uniform:UNIFORM_NORMAL_MATRIX], 1, 0, self.normalMatrix.m);
    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_LIGHT_VIEWPROJECTION], 1,0, self.lightViewProjection.m);
    
    glUniform4fv([self.glProgram uniformWithStr:@"material.ambient"], 1, self.material.ambient.v);
    glUniform4fv([self.glProgram uniformWithStr:@"material.diffuse"], 1, self.material.diffuse.v);
    glUniform4fv([self.glProgram uniformWithStr:@"material.specular"], 1, self.material.specular.v);
    
    glUniform1i([self.glProgram uniform:UNIFORM_DIFFUSE_MAP], 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self.material.diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, self.material.shadowMap);
    glUniform1i([self.glProgram uniform:UNIFORM_SHADOW_MAP], 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, self.material.normalMap);
    glUniform1i([self.glProgram uniformWithStr:@"normalMap"], 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, self.material.specularMap);
    glUniform1i([self.glProgram uniformWithStr:@"specularMap"], 3);
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, self.material.ambientMap);
    glUniform1i([self.glProgram uniformWithStr:@"ambientMap"], 4);

    glBindVertexArrayOES(self.vao);
    if (self.data.supportIndiceVBO) {
        glDrawElements(GL_TRIANGLES, self.data.indiceCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, self.data.vertexCount);
    }
    glBindVertexArrayOES(0);
}

- (void)update:(NSTimeInterval)interval {

    //self.transform.quaternion = GLKQuaternionMakeWithAngleAndAxis(rotation, 0, 1, 0);
    rotation += interval * 0.8f;

    elapsedTime += interval;
    if (elapsedTime >= 1 / 30.0f) {
        currentTexture++;
        if (currentTexture > self.textures.count - 1) {
            currentTexture = 0;
            elapsedTime = 0;
        }
    }
}

- (GLKMatrix4)modelMatrix {
    return [self.transform matrix];
}

- (GLKMatrix3)normalMatrix {
    GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(self.modelMatrix), NULL);
    return normalMatrix;
}

- (EZGLProgram *)glProgram {
    if (_glProgram != nil && _glProgram.isValid) {
        return _glProgram;
    }
    return self.world.effect.program;
}

// Override this to use different View Projection
- (GLKMatrix4)viewProjection {
    return self.world.camera.matrix;
}

- (NSArray *)rigidBodys {
    return [NSArray new];
}

@end
