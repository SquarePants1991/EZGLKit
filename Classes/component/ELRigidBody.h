//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELRIGIDBODY_H
#define EZGL_ELRIGIDBODY_H

#include "../types/EZGLTypes.h"
#include "../core/ELComponent.h"

class ELCollisionShape;
class btRigidBody;
class btTransform;

class ELRigidBody : public ELComponent {
public:
    ELCollisionShape *collisionShape;
    ELFloat mass;
    ELVector3 velocity;
    ELFloat restitution;
    ELFloat friction;
    ELVector3 angleFactor;
    ELVector3 linearFactor;
    ELInt collisionGroup;
    ELInt collisionMask;

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
