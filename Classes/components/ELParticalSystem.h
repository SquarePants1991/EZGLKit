//
// Created by wangyang on 16/12/6.
//

#ifndef EZGL_ELPARTICALSYSTEM_H
#define EZGL_ELPARTICALSYSTEM_H

#include "EZGLBase.h"
#include "ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELPartical;

class ELParticalSystem : public ELGeometry {
public:
    ELParticalSystem();

    virtual ELGeometryData generateData();
    virtual void update(ELFloat timeInSecs);
    virtual void render();

private:
    ELGeometryVertexBuffer *vertexBuffer;
    ELGeometryData currentData;
    bool isDataInitialized;
    ELPartical *partical;
};


#endif //EZGL_ELPARTICALSYSTEM_H
