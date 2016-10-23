//
//  EZGLPhysicsWorld.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLPhysicsWorld.h"
#import "btBulletDynamicsCommon.h"
#import "btBulletCollisionCommon.h"

#import "EZGLGeometry.h"
#import "EZGLRigidBody.h"
#import "EZGLTransform.h"

@interface EZGLPhysicsWorld () {
    btDiscreteDynamicsWorld *world;
    btCollisionConfiguration *configration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btBroadphaseInterface *broadphase;
}
@property (strong, nonatomic) NSMutableArray *rigidBodies;
@end

@implementation EZGLPhysicsWorld

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.rigidBodies = [NSMutableArray new];
        
        configration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(configration);
        solver = new btSequentialImpulseConstraintSolver();
        broadphase = new btDbvtBroadphase();
        
        world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,configration);
        world->setGravity(btVector3(0,-10,0));
    }
    return self;
}

- (void)createRigidbody:(float)mass geometry:(EZGLGeometry *)geometry {
    NSArray *bodies = [geometry rigidBodys];
    for (EZGLRigidBody *body in bodies) {
        world->addRigidBody(((btRigidBody *)[body rigidBody]));
        [self.rigidBodies addObject:body];
    }
}

- (void)update:(NSTimeInterval)interval {
    world->stepSimulation(interval);
    
    for (EZGLRigidBody *body in self.rigidBodies) {
        if (body.geometry != nil) {
            [body sync];
        }
    }
}


- (void)render:(CGRect)rect {
    world->debugDrawWorld();
}

@end
