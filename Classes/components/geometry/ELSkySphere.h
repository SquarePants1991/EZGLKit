//
// Created by wangyang on 16/12/22.
//

#ifndef EZGL_ELSKYSPHERE_H
#define EZGL_ELSKYSPHERE_H

#include "ELVertexBufferGeometry.h"

class ELSkySphere : public ELVertexBufferGeometry {
public:
    ELSkySphere(ELFloat radius);

    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer);
private:
    ELFloat radius;
    ELFloat ring;
    ELFloat segments;

    void genHalfSphere(ELInt step, ELGeometryVertexBuffer *vertexBuffer);
};


#endif //EZGL_ELSKYSPHERE_H
