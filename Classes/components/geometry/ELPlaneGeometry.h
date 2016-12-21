//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELPLANGEOMETRY_H
#define EZGL_ELPLANGEOMETRY_H

#include "ELVertexBufferGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELPlaneGeometry : public ELVertexBufferGeometry {
public:
    ELPlaneGeometry(ELVector2 size);
    ELPlaneGeometry(ELVector2 size, bool smooth);

    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer);

private:
    ELVector2 size;
};


#endif //EZGL_ELPLANGEOMETRY_H
