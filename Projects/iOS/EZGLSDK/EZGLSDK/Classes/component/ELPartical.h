//
// Created by wangyang on 16/12/6.
//

#ifndef EZGL_ELPARTICAL_H
#define EZGL_ELPARTICAL_H

#include "EZGLBase.h"
#include "types/EZGLTypes.h"

class ELPartical {
public:
    ELVector3 relativePosition;
    ELTransform transform;

    ELVector3 velocity;

    ELVector2 size;
    ELVector2 sizeStart;
    ELVector2 sizeEnd;

    ELVector4 color;
    ELVector4 colorStart;
    ELVector4 colorEnd;

    ELFloat rotation;
    ELFloat rotationStart;
    ELFloat rotationEnd;

    ELFloat age;

    ELGeometryColorRect quadRect;

    ELPartical();

    void update(ELVector3 cameraPosition,ELFloat timeInSecs,ELVector3 force);
    void reset();
    bool isAlive();
private:
    ELFloat currentAge;
};


#endif //EZGL_ELPARTICAL_H
