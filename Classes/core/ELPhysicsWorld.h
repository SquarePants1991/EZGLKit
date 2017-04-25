//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELPHYSICSWORLD_H
#define EZGL_ELPHYSICSWORLD_H

#include "ELNode.h"

class ELGameObject;
class btRigidBody;
class btDiscreteDynamicsWorld;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btBroadphaseInterface;

// TODO: 使用shared_ptr管理
class ELPhysicsWorld : public ELNode{
public:
    ELPhysicsWorld();
    ~ELPhysicsWorld();
    void setGravity(ELVector3 gravity);
    void addRigidBody(btRigidBody *rigidBody);
    void addRigidBody(btRigidBody *rigidBody, ELInt group, ELInt mask);
    void removeRigidBody(btRigidBody *rigidBody);

    std::vector<ELGameObject *> raycast(ELVector3 start,ELVector3 end);
    std::vector<ELGameObject *> raycast(ELVector3 start,ELVector3 end, ELInt filterMask);

    virtual void update(ELFloat timeInSecs);
private:
    btDiscreteDynamicsWorld *world;
    btCollisionConfiguration *configration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btBroadphaseInterface *broadphase;
};


#endif //EZGL_ELPHYSICSWORLD_H
