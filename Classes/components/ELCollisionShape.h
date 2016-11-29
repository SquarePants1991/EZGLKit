//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELCOLLISIONSHAPE_H
#define EZGL_ELCOLLISIONSHAPE_H


#include "types/EZGLTypes.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class ELCollisionShape {
public:
    ELCollisionShape();

    void asSphere(ELFloat radius);
    void asBox(ELVector3 boxMargin);
    void asCone(ELFloat radius, ELFloat height);
    void asStaticPlane();
//    void asTerrian();

    btCollisionShape *collisionShape;
};


#endif //EZGL_ELCOLLISIONSHAPE_H
