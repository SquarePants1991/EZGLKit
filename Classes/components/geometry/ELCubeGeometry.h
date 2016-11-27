//
// Created by wang yang on 2016/11/27.
//

#ifndef EZGL_ELCUBEGEOMETRY_H
#define EZGL_ELCUBEGEOMETRY_H

#include "../ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELCubeGeometry : public ELGeometry {
public:
    ELCubeGeometry(ELVector3 size);
    ~ELCubeGeometry();

    virtual void update(ELFloat timeInSecs);
    virtual ELGeometryData generateData();

private:
    ELVector3 size;
    ELGeometryVertexBuffer *vertexBuffer;
};


#endif //EZGL_ELCUBEGEOMETRY_H
