//
// Created by wangyang on 16/11/29.
//

#include "ELPhysicsWorld.h"
#include "ELGameObject.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

ELPhysicsWorld::ELPhysicsWorld() {
    configration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(configration);
    solver = new btSequentialImpulseConstraintSolver();
    broadphase = new btDbvtBroadphase();

    world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,configration);
    world->setGravity(btVector3(0,-25,0));

    kind = "physics world";
}

ELPhysicsWorld::~ELPhysicsWorld() {
    delete(world);
}

void ELPhysicsWorld::setGravity(ELVector3 gravity) {
    world->setGravity(btVector3(gravity.x,gravity.y,gravity.z));
}

void ELPhysicsWorld::addRigidBody(btRigidBody *rigidBody) {
    world->addRigidBody(rigidBody);
}

void ELPhysicsWorld::addRigidBody(btRigidBody *rigidBody, ELInt group, ELInt mask) {
    world->addRigidBody(rigidBody, group, mask);
}

void ELPhysicsWorld::removeRigidBody(btRigidBody *rigidBody) {
    world->removeRigidBody(rigidBody);
}

std::vector<ELGameObject *> ELPhysicsWorld::raycast(ELVector3 start,ELVector3 end) {
    std::vector<ELGameObject *> objects;
    btVector3 startVec = btVector3(start.x,start.y,start.z);
    btVector3 endVec = btVector3(end.x,end.y,end.z);
    btCollisionWorld::ClosestRayResultCallback rayCallback(startVec, endVec);
    world->rayTest(startVec,endVec,rayCallback);
    if(rayCallback.hasHit()) {
        btVector3 end = rayCallback.m_hitPointWorld;
        btVector3 normal = rayCallback.m_hitNormalWorld;
        ELGameObject *gameObj = (ELGameObject *)(rayCallback.m_collisionObject->getUserPointer());
        objects.push_back(gameObj);
    }
    return objects;
}

std::vector<ELGameObject *> ELPhysicsWorld::raycast(ELVector3 start,ELVector3 end, ELInt filterMask) {
    std::vector<ELGameObject *> objects;
    btVector3 startVec = btVector3(start.x,start.y,start.z);
    btVector3 endVec = btVector3(end.x,end.y,end.z);
    btCollisionWorld::ClosestRayResultCallback rayCallback(startVec, endVec);
    rayCallback.m_collisionFilterMask = filterMask;
    world->rayTest(startVec,endVec,rayCallback);
    if(rayCallback.hasHit()) {
        btVector3 end = rayCallback.m_hitPointWorld;
        btVector3 normal = rayCallback.m_hitNormalWorld;
        ELGameObject *gameObj = (ELGameObject *)(rayCallback.m_collisionObject->getUserPointer());
        objects.push_back(gameObj);
    }
    return objects;
}

void ELPhysicsWorld::update(ELFloat timeInSecs) {
    world->stepSimulation(timeInSecs);
}
