//
//  GLRigidBody.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GLPhysicsWorld.h"

@class GLGeometry;

@interface GLRigidBody : NSObject

@property (strong, nonatomic) GLGeometry *geometry;

- (instancetype)initAsSphere:(float)radius mass:(float)mass geometry:(GLGeometry *)geometry;
- (instancetype)initAsStaticPlane:(float)size geometry:(GLGeometry *)geometry;
- (void)sync;
- (void *)rigidBody;

@end
