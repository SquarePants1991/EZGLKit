//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGEOMETRYVERTEXBUFFER_H
#define EZGL_ELGEOMETRYVERTEXBUFFER_H

#include "types/ELGeometryTypes.h"

class ELGeometryVertexBuffer {
public:
    ELGeometryVertexBuffer();
    void append(ELGeometryVertex vertex);
    void append(ELGeometryTriangle triangle);
    void append(ELGeometryRect rect);
    ELSize rawLength();
    void caculatePerVertexNormal();
    void * data();
    void clear();

private:
    ELInt bufferLen;
    ELInt index;
    ELGeometryVertex *vertices;

    void caculateTangents(ELVector3 *pTangent, ELVector3 *pBitangent, ELVector3 position, ELVector3 normal);
};


#endif //EZGL_ELGEOMETRYVERTEXBUFFER_H
