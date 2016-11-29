//
// Created by wangyang on 16/11/29.
//

#include "ELCollisionShape.h"

ELCollisionShape::ELCollisionShape() {

}

void ELCollisionShape::asSphere(ELFloat radius) {
    btCollisionShape *shape = new btSphereShape(radius);
    collisionShape = shape;
}

void ELCollisionShape::asBox(ELVector3 boxMargin) {
    btCollisionShape *shape = new btBoxShape(btVector3(boxMargin.x, boxMargin.y, boxMargin.z));
    collisionShape = shape;
}

void ELCollisionShape::asCone(ELFloat radius, ELFloat height) {
    btCollisionShape *shape = new btConeShape(radius, height);
    collisionShape = shape;
}

void ELCollisionShape::asStaticPlane() {
    btCollisionShape *shape = new btStaticPlaneShape(btVector3(0, 1, 0),0);
    collisionShape = shape;
}