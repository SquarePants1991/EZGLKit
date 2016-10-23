//
//  EZGLRigidBody.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EZGLPhysicsWorld.h"

@class EZGLGeometry;

@interface EZGLRigidBody : NSObject

@property (strong, nonatomic) EZGLGeometry *geometry;

- (instancetype)initAsSphere:(float)radius mass:(float)mass geometry:(EZGLGeometry *)geometry;
- (instancetype)initAsStaticPlane:(float)size geometry:(EZGLGeometry *)geometry;
- (void)sync;
- (void *)rigidBody;

@end
