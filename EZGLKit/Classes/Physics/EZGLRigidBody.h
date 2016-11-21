//
//  EZGLRigidBody.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <GLKit/GLKit.h>
#import "EZGLPhysicsWorld.h"

@class EZGLGeometry;

@interface EZGLRigidBody : NSObject

@property (strong, nonatomic) EZGLGeometry *geometry;

- (instancetype)initAsSphere:(float)radius mass:(float)mass geometry:(EZGLGeometry *)geometry;
- (instancetype)initAsTerrain:(float *)data size:(CGSize)size minHeight:(float)minHeight maxHeight:(float)maxHeight geometry:(EZGLGeometry *)geometry;
- (instancetype)initAsBox:(GLKVector3)halfMargins mass:(float)mass geometry:(EZGLGeometry *)geometry;
- (instancetype)initAsCone:(GLfloat)radius height:(GLfloat)height mass:(float)mass geometry:(EZGLGeometry *)geometry;
- (instancetype)initAsStaticPlane:(float)size geometry:(EZGLGeometry *)geometry;
- (void)sync;
- (void *)rigidBody;
- (void)setVelocity:(GLKVector3)velocity;
@end
