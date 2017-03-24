//
// Created by wangyang on 16/11/29.
//

#ifndef EZGL_ELCOLLISIONSHAPE_H
#define EZGL_ELCOLLISIONSHAPE_H


#include "../types/EZGLTypes.h"
class btCollisonShape;

class ELCollisionShape {
public:
    ELCollisionShape();
    ELVector3 offset;
    void asSphere(ELFloat radius);
    void asBox(ELVector3 boxMargin);
    void asCone(ELFloat radius, ELFloat height);
    void asStaticPlane();
    void asTerrian(ELFloat *data,ELVector2 size,ELFloat minHeight,ELFloat maxHeight);

    btCollisionShape *collisionShape;
};


#endif //EZGL_ELCOLLISIONSHAPE_H
