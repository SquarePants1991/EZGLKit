//
// Created by wangyang on 16/11/29.
//

#include "ELPhysicsWorld.h"

ELPhysicsWorld * ELPhysicsWorld::_shared = NULL;

ELPhysicsWorld * ELPhysicsWorld::shared() {
    if (_shared == NULL) {
        _shared = new ELPhysicsWorld();
    }
    return _shared;
}

ELPhysicsWorld::ELPhysicsWorld() {
    configration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(configration);
    solver = new btSequentialImpulseConstraintSolver();
    broadphase = new btDbvtBroadphase();

    world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,configration);
    world->setGravity(btVector3(0,-10,0));
}

void ELPhysicsWorld::addRigidBody(btRigidBody *rigidBody) {
    world->addRigidBody(rigidBody);
}

void ELPhysicsWorld::update(ELFloat timeInSecs) {
    world->stepSimulation(timeInSecs);
}