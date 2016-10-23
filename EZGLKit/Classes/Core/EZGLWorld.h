//
//  EZGLWorld.h
//  OpenESRD
//
//  Created by wang yang on 2016/10/7.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/glext.h>
#import <GLKit/GLKit.h>
#import "EZGLPerspectiveCamera.h"
#import "EZGLOrthoCamera.h"

@class EZGLGeometry;
@class EZGLLight;
@class EZGLPhysicsWorld;
@class EZGLCamera;

@interface EZGLWorld : NSObject

@property (strong, nonatomic) EAGLContext *context;
@property (assign, nonatomic) GLKMatrix4 viewProjection;
@property (strong, nonatomic) EZGLLight *light;
@property (assign, nonatomic) GLKMatrix4 lightViewProjection;
@property (strong, nonatomic) EZGLPhysicsWorld *physicsWorld;

@property (assign, nonatomic) CGFloat angleY;
@property (assign, nonatomic) CGFloat angleX;

@property (strong, nonatomic) EZGLCamera *camera;

- (instancetype)initWithGLKView:(GLKView *)glkView;
- (void)addGeometry:(EZGLGeometry *)geometry;
- (void)render:(CGRect)rect;
- (void)update:(NSTimeInterval)interval;
@end
