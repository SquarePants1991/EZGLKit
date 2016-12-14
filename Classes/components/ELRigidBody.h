//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELRIGIDBODY_H
#define EZGL_ELRIGIDBODY_H

#include "types/EZGLTypes.h"
#include "core/ELComponent.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class ELCollisionShape;

class ELRigidBody : public ELComponent {
public:
    ELCollisionShape *collisionShape;
    ELFloat mass;
    ELVector3 velocity;
public:
    ELRigidBody(ELCollisionShape *shape, ELFloat mass);
    void applyForce(ELVector3 force,ELVector3 pos);
    void applyDamping(ELFloat timeStep);
    void setVelocity(ELVector3 velocity);
    void setVelocityX(ELFloat velocityX);
    void setVelocityY(ELFloat velocityY);
    void setVelocityZ(ELFloat velocityZ);

    virtual void update(ELFloat timeInSecs);
    virtual void didAddedToGameObject(ELGameObject *gameObject);

protected:
    ~ELRigidBody();
private:
    btRigidBody *rigidBody;
    btTransform btTransformFromELTransform(ELTransform transform);
    ELTransform elTransformFrombtTransform(btTransform transform,ELTransform originELTransform);
};


#endif //EZGL_ELRIGIDBODY_H
