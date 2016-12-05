//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELPHYSICSWORLD_H
#define EZGL_ELPHYSICSWORLD_H

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "ELNode.h"

class ELGameObject;

class ELPhysicsWorld : public ELNode{
public:
    static ELPhysicsWorld *shared();
    void addRigidBody(btRigidBody *rigidBody);

    std::vector<ELGameObject *> raycast(ELVector3 start,ELVector3 end);

    virtual void update(ELFloat timeInSecs);
private:
    ELPhysicsWorld();

private:
    static ELPhysicsWorld *_shared;
    btDiscreteDynamicsWorld *world;
    btCollisionConfiguration *configration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btBroadphaseInterface *broadphase;
};


#endif //EZGL_ELPHYSICSWORLD_H
