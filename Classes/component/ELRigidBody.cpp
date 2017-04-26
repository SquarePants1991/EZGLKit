//
// Created by wangyang on 16/11/29.
//

#include "ELRigidBody.h"
#include "ELCollisionShape.h"
#include "../core/ELGameObject.h"
#include "../core/ELPhysicsWorld.h"

#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

ELRigidBody::ELRigidBody(std::shared_ptr<ELCollisionShape> shape, ELFloat mass) :
        mass(mass),
        friction(0.1),
        restitution(0.2),
        angleFactor(ELVector3Make(1,1,1)),
        linearFactor(ELVector3Make(1,1,1)),
        collisionGroup(0x00000001),
        collisionMask(0x00000001)
{
    this->kind = "rigidbody";
    this->collisionShape = shape;
}

ELRigidBody::~ELRigidBody() {
}

void ELRigidBody::applyForce(ELVector3 force,ELVector3 pos) {
    rigidBody->setActivationState(ACTIVE_TAG);
    rigidBody->applyCentralForce(btVector3(force.x,force.y,force.z));
}

void ELRigidBody::applyDamping(ELFloat timeStep) {
    rigidBody->applyDamping(timeStep);
}

void ELRigidBody::setVelocity(ELVector3 velocity) {
    rigidBody->setActivationState(ACTIVE_TAG);
    rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    this->velocity = velocity;
}

void ELRigidBody::setVelocityX(ELFloat velocityX) {
    rigidBody->setActivationState(ACTIVE_TAG);
    btVector3 velocityOrigin = rigidBody->getLinearVelocity();
    rigidBody->setLinearVelocity(btVector3(velocityX, velocityOrigin.y(), velocityOrigin.z()));
    this->velocity = ELVector3Make(velocityX, velocity.y, velocity.z);
}

void ELRigidBody::setVelocityY(ELFloat velocityY) {
    rigidBody->setActivationState(ACTIVE_TAG);
    btVector3 velocityOrigin = rigidBody->getLinearVelocity();
    rigidBody->setLinearVelocity(btVector3(velocityOrigin.x(), velocityY, velocityOrigin.z()));
    this->velocity = ELVector3Make(velocity.x, velocityY, velocity.z);
}

void ELRigidBody::setVelocityZ(ELFloat velocityZ) {
    rigidBody->setActivationState(ACTIVE_TAG);
    btVector3 velocityOrigin = rigidBody->getLinearVelocity();
    rigidBody->setLinearVelocity(btVector3(velocityOrigin.x(), velocityOrigin.y(), velocityZ));
    this->velocity = ELVector3Make(velocity.x, velocity.y, velocityZ);
}

void ELRigidBody::didAddedToGameObject(ELGameObject *gameObject) {

    btTransform defaultTransform = btTransformFromELTransform(*(gameObject->transform));
    btDefaultMotionState *motionState = new btDefaultMotionState(defaultTransform);
    btVector3 fallInertia(0,0,0);
    collisionShape->collisionShape->calculateLocalInertia(mass,fallInertia);
    rigidBody = retain_ptr_init_v(btRigidBody, mass,motionState,collisionShape->collisionShape.get(),fallInertia);
    rigidBody->setRestitution(restitution);
    rigidBody->setFriction(friction);

    rigidBody->setAngularFactor(btVector3(angleFactor.x,angleFactor.y,angleFactor.z));
    rigidBody->setLinearFactor(btVector3(linearFactor.x, linearFactor.y, linearFactor.z));
    this->gameObject()->world.lock()->physicsWorld->addRigidBody(rigidBody.get(), collisionGroup, collisionMask);
    rigidBody->setUserPointer(this->gameObject());
    collisionShape->collisionShape->setUserPointer(this->gameObject());
}

void ELRigidBody::update(ELFloat timeInSecs) {
    ELGameObject *gameObj = (ELGameObject *)gameObject();
    ELTransform elTransform = elTransformFrombtTransform(rigidBody->getWorldTransform(),*(gameObj->transform));
    gameObj->transform->position = elTransform.position;
    gameObj->transform->quaternion = elTransform.quaternion;
}

// btTransform + offset = elTransform

btTransform ELRigidBody::btTransformFromELTransform(ELTransform transform) {
    ELQuaternion elQuaternion = transform.quaternion;
    btQuaternion quaternion(elQuaternion.x,elQuaternion.y,elQuaternion.z,elQuaternion.w);
    btTransform bttransform(quaternion,btVector3(transform.position.x - collisionShape->offset.x, transform.position.y - collisionShape->offset.y, transform.position.z - collisionShape->offset.z));
    return bttransform;
}

ELTransform ELRigidBody::elTransformFrombtTransform(btTransform transform,ELTransform originELTransform) {
    ELTransform elTransform = originELTransform;
    elTransform.position.x = transform.getOrigin().x() + collisionShape->offset.x;
    elTransform.position.y = transform.getOrigin().y() + collisionShape->offset.y;
    elTransform.position.z = transform.getOrigin().z() + collisionShape->offset.z;

    btQuaternionFloatData quaternionFloatData;
    transform.getRotation().serialize(quaternionFloatData);

    elTransform.quaternion = ELQuaternionMake((float)quaternionFloatData.m_floats[0], (float)quaternionFloatData.m_floats[1], (float)quaternionFloatData.m_floats[2], (float)quaternionFloatData.m_floats[3]);
    return elTransform;
}
