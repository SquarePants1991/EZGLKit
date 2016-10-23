//
//  Geometry.m
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "GLGeometry.h"
#import "UIImage+GL.h"
#import "GLWaveFrontFile.h"
#import "GLDefines.h"
#import "GLLight.h"
#import "GLWorld.h"

@interface GLGeometry () {
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

@implementation GLGeometry

- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader {
    self = [super init];
    if (self) {
        self.vertexShader = vertexShader;
        self.fragmentShader = fragmentShader;
    }
    return self;
}

- (void)setupWithData:(GLGeometryData)data {
    self.data = data;
    if (self.vertexShader == nil) {
        self.vertexShader = @"Shader";
    }
    if (self.fragmentShader == nil) {
        self.fragmentShader = @"Shader";
    }
    self.glProgram = [[GLProgram alloc]initWithVertexShaderFileName:self.vertexShader fragmentShaderFileName:self.fragmentShader];
    self.material = [GLMaterial new];
    [self createTexture];
    [self setupVAO];
    [self setupTransform];
}

- (void)setupDefaultMaterial {
    
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

    GLuint uvLocation = glGetAttribLocation(self.glProgram.value, "uv");
    glEnableVertexAttribArray(uvLocation);
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, self.data.vertexStride, BUFFER_OFFSET(6 * sizeof(GLfloat)));

    if (self.data.indiceVBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.data.indiceVBO);
    }

    glBindVertexArrayOES(0);
}

- (void)setupTransform {
    self.transform = [GLTransform new];
}

- (void)createTexture {
    self.textures = [UIImage texturesFromGif:@"demo"];
    currentTexture = 0;
}

- (void)draw {
    glUseProgram(self.glProgram.value);

    GLint renderAsShadow = self.renderAsShadow ? 1 : 0;
    glUniform1i([self.glProgram uniform:UNIFORM_RENDERASSHADOW], renderAsShadow);

    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_VIEWPROJECTION], 1, 0, self.viewProjection.m);
    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_MODEL_MATRIX], 1, 0, self.modelMatrix.m);
    glUniformMatrix3fv([self.glProgram uniform:UNIFORM_NORMAL_MATRIX], 1, 0, self.normalMatrix.m);
    glUniform4fv([self.glProgram uniform:UNIFORM_AMBIENT], 1, self.material.ambient.v);
    glUniform4fv([self.glProgram uniform:UNIFORM_DIFFUSE], 1, self.material.diffuse.v);
    glUniform4fv([self.glProgram uniform:UNIFORM_SPECULAR], 1, self.material.specular.v);
    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_LIGHT_VIEWPROJECTION], 1,0, self.lightViewProjection.m);
    glUniform4fv([self.glProgram uniform:UNIFORM_LIGHT_COLOR], 1, self.world.light.color.v);
    glUniform1f([self.glProgram uniform:UNIFORM_LIGHT_BRIGHTNESS], self.world.light.brightness);
    glUniform3fv([self.glProgram uniform:UNIFORM_LIGHT_POSITION], 1, self.world.light.position.v);

    glUniform1i([self.glProgram uniform:UNIFORM_DIFFUSE_MAP], 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self.material.diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, self.material.shadowMap);
    glUniform1i([self.glProgram uniform:UNIFORM_SHADOW_MAP], 1);

    glBindVertexArrayOES(self.vao);
    if (self.data.supportIndiceVBO) {
        glDrawElements(GL_TRIANGLES, self.data.indiceCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, self.data.vertexCount);
    }
    glBindVertexArrayOES(0);
}

- (void)update:(NSTimeInterval)interval {

    //rotation += interval * 0.8f;

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
    GLKMatrix4 mvp = GLKMatrix4Multiply(self.viewProjection, self.modelMatrix);
    GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(mvp), NULL);
    return normalMatrix;
}

- (NSArray *)rigidBodys {
    return [NSArray new];
}

@end
