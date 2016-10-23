//
//  GLWorld.m
//  OpenESRD
//
//  Created by wang yang on 2016/10/7.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "GLWorld.h"
#import "GLGeometry.h"
#import "GLLight.h"
#import "GLPlaneGeometry.h"
#import "UIImage+GL.h"
#import "GLPhysicsWorld.h"

@interface GLWorld ()

@property (strong, nonatomic) NSMutableArray *geometrys;
@property (assign, nonatomic) GLKMatrix4 originViewProjection;
@property (weak, nonatomic) GLKView *glkView;

@property (strong, nonatomic) GLPlaneGeometry *projector;
@property (assign, nonatomic) GLuint shadowFramebuffer;
@property (assign, nonatomic) GLuint shadowTexture;
@property (assign, nonatomic) GLuint testTexture;

@end

@implementation GLWorld

- (instancetype)initWithGLKView:(GLKView *)glkView {
    self = [super init];
    if (self) {

        self.glkView = glkView;

        CGSize size = glkView.bounds.size;

        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

        if (!self.context) {
            NSLog(@"Failed to create ES context");
        }

        glkView.context = self.context;
        glkView.drawableDepthFormat = GLKViewDrawableDepthFormat24;

        [EAGLContext setCurrentContext:self.context];
        glEnable(GL_DEPTH_TEST);

        float aspect = fabs(size.width / size.height);
        self.viewProjection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
//        self.viewProjection = GLKMatrix4MakeFrustum(-aspect, aspect, -1, 1, 0.1, 1000);
        GLKMatrix4 cameraLookAt = GLKMatrix4MakeLookAt(0, 20, 20, 0, 0, 0, 0, 1, 0);
        self.viewProjection = GLKMatrix4Multiply(self.viewProjection, cameraLookAt);
        self.originViewProjection = self.viewProjection;

        self.light = [GLLight new];
        CGRect rect = self.glkView.bounds;
        self.lightViewProjection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
//        self.lightViewProjection = GLKMatrix4MakeOrtho(-1, 1, -1, 1, 0.1, 100);
//        self.lightViewProjection = GLKMatrix4MakeFrustum(-1, 1, -1, 1, 0.1, 100);
        GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(self.light.position.x, self.light.position.y, self.light.position.z, 0, 0, 0, 0, 1, 0);
        self.lightViewProjection = GLKMatrix4Multiply(self.lightViewProjection, lookAt);
        [self createShadowFrameBuffer];

        self.geometrys = [NSMutableArray new];

        self.projector = [[GLPlaneGeometry alloc] initWithVertexShader:@"ShadowMap" fragmentShader:@"ShadowMap"];
        self.projector.world = self;
        
        self.physicsWorld = [GLPhysicsWorld new];
    }
    return self;
}

- (void)createShadowFrameBuffer {
    // create framebuffer
    GLuint framebuffer;
    GLuint renderbuffer;
    GLuint shadowTexture;

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &shadowTexture);
    glGenRenderbuffers(1, &renderbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024, 1024);

    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    self.shadowFramebuffer = framebuffer;
    self.shadowTexture = shadowTexture;
    self.testTexture = [UIImage textureFromCGImage:[UIImage imageNamed:@"texture.png"].CGImage];
}

- (void)addGeometry:(GLGeometry *)geometry {
    geometry.world = self;
    [self.geometrys addObject:geometry];
    [self.physicsWorld createRigidbody:1.0f geometry:geometry];
}

- (void)render:(CGRect)rect {
    float aspect = fabs(rect.size.width / rect.size.height);

    GLKMatrix4 projection = GLKMatrix4RotateX(self.originViewProjection, self.angleX);
    projection = GLKMatrix4RotateY(projection, self.angleY);
    self.viewProjection = projection;

    self.lightViewProjection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), 1, 0.1f, 100.0f);
//    self.lightViewProjection = GLKMatrix4MakeFrustum(-1, 1, -1, 1, 0.1, 400);
    GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(self.light.position.x, self.light.position.y, self.light.position.z, 0, 0, 0, 0, 1, 0);
    self.lightViewProjection = GLKMatrix4Multiply(self.lightViewProjection, lookAt);

    glBindFramebuffer(GL_FRAMEBUFFER, self.shadowFramebuffer);
    glViewport(0, 0, 1024, 1024);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    
    for (GLGeometry *geometry in self.geometrys) {
        geometry.viewProjection = self.lightViewProjection;
        geometry.lightViewProjection = self.lightViewProjection;
        geometry.renderAsShadow = YES;
        [geometry draw];
    }

    [self.glkView bindDrawable];
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (GLGeometry *geometry in self.geometrys) {
        geometry.viewProjection = self.viewProjection;
        geometry.lightViewProjection = self.lightViewProjection;
        geometry.renderAsShadow = NO;
        geometry.material.shadowMap = self.shadowTexture;
        [geometry draw];
    }

    self.projector.viewProjection = GLKMatrix4MakeOrtho(-rect.size.width / 2, rect.size.width / 2, -rect.size.height / 2, rect.size.height / 2, -1.0, 100.0f);
    self.projector.viewProjection = GLKMatrix4Translate(self.projector.viewProjection, -rect.size.width / 2 + 50, rect.size.height / 2 - 50, 1);
    self.projector.viewProjection = GLKMatrix4Scale(self.projector.viewProjection, 100, 100, 1);
    self.projector.renderAsShadow = NO;
    self.projector.material.diffuseMap = self.shadowTexture;
    [self.projector draw];
    
    [self.physicsWorld render:rect];
}

- (void)update:(NSTimeInterval)interval {
    [self.physicsWorld update:interval];
    [self.light update:interval];
    for (GLGeometry *geometry in self.geometrys) {
        [geometry update:interval];
    }
    [self.projector update:interval];
}

@end
