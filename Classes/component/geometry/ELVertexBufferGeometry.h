//
// Created by wangyang on 16/12/21.
//

#ifndef EZGL_ELVERTEXBUFFERGEOMETRY_H
#define EZGL_ELVERTEXBUFFERGEOMETRY_H

#include "component/ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELVertexBufferGeometry : public ELGeometry {
public:
    ELVertexBufferGeometry();
    ELVertexBufferGeometry(bool smooth);
    ~ELVertexBufferGeometry();

    virtual ELGeometryData generateData();
    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) = 0;
    virtual void effectDidActive(ELEffect * effect){}
    virtual void effectDidInactive(ELEffect * effect){}

protected:
    bool smooth;
    ELGeometryVertexBuffer *vertexBuffer;
private:
};


#endif //EZGL_ELVERTEXBUFFERGEOMETRY_H
