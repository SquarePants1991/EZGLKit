//
// Created by wang yang on 2016/11/27.
//

#ifndef EZGL_ELCUBEGEOMETRY_H
#define EZGL_ELCUBEGEOMETRY_H

#include "ELVertexBufferGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELCubeGeometry : public ELVertexBufferGeometry {
public:
    ELCubeGeometry(ELVector3 size);
    ELCubeGeometry(ELVector3 size, bool smooth);

    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer);

private:
    ELVector3 size;
};


#endif //EZGL_ELCUBEGEOMETRY_H
