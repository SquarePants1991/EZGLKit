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
#import "EZGLProgram.h"
#import "EZGLEffect.h"

@class EZGLGeometry;
@class EZGLLight;
@class EZGLPhysicsWorld;
@class EZGLCamera;

@interface EZGLWorld : NSObject

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) EZGLPhysicsWorld *physicsWorld;

@property (strong, nonatomic) EZGLEffect *effect;
@property (strong, nonatomic) EZGLCamera *camera;

@property (assign, nonatomic) BOOL physicsEnabled;

- (instancetype)initWithGLKView:(GLKView *)glkView;
- (void)addNode:(EZGLNode *)node;
- (void)render:(CGRect)rect;
- (void)update:(NSTimeInterval)interval;
@end