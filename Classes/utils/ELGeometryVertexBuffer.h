//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGEOMETRYVERTEXBUFFER_H
#define EZGL_ELGEOMETRYVERTEXBUFFER_H

#include "types/ELGeometryTypes.h"

class ELGeometryVertexBuffer {
public:
    bool supportColorAttrib;
public:
    ELGeometryVertexBuffer();
    void enableColorAttrib();
    void append(ELGeometryVertex vertex);
    void append(ELGeometryColorVertex vertex);
    void append(ELGeometryTriangle triangle);
    void append(ELGeometryRect rect);
    void append(ELGeometryColorRect colorRect);
    ELSize rawLength();
    void caculatePerVertexNormal();
    void * data();
    void clear();

private:
    ELInt bufferLen;
    ELInt index;
    ELGeometryVertex *vertices;
    ELGeometryColorVertex *colorVertices;

    void caculateTangents(ELVector3 *pTangent, ELVector3 *pBitangent, ELVector3 position, ELVector3 normal);
};


#endif //EZGL_ELGEOMETRYVERTEXBUFFER_H
