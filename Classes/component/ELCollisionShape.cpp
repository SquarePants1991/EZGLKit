//
// Created by wangyang on 16/11/29.
//

#include "ELCollisionShape.h"

#include "bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

ELCollisionShape::ELCollisionShape() {
    offset = ELVector3Make(0, 0, 0);
}

void ELCollisionShape::asSphere(ELFloat radius) {
    btCollisionShape *shape = new btSphereShape(radius);
    collisionShape = retain_ptr(btCollisionShape, shape);
}

void ELCollisionShape::asBox(ELVector3 boxMargin) {
    btCollisionShape *shape = new btBoxShape(btVector3(boxMargin.x, boxMargin.y, boxMargin.z));
    collisionShape = retain_ptr(btCollisionShape, shape);
}

void ELCollisionShape::asCone(ELFloat radius, ELFloat height) {
    btCollisionShape *shape = new btConeShape(radius, height);
    collisionShape = retain_ptr(btCollisionShape, shape);
}

void ELCollisionShape::asStaticPlane() {
    btCollisionShape *shape = new btStaticPlaneShape(btVector3(0, 1, 0),0);
    collisionShape = retain_ptr(btCollisionShape, shape);
}

void ELCollisionShape::asTerrian(ELFloat *data,ELVector2 size,ELFloat minHeight,ELFloat maxHeight) {
    btCollisionShape *shape = new btHeightfieldTerrainShape(size.x,size.y,data,1,minHeight,maxHeight,1,PHY_FLOAT,false);
    collisionShape = retain_ptr(btCollisionShape, shape);
}
