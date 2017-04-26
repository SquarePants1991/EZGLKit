//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGEOMETRYVERTEXBUFFER_H
#define EZGL_ELGEOMETRYVERTEXBUFFER_H

#include "../EZGLBase.h"
#include "../types/ELGeometryTypes.h"

union _AABBBox{
    ELFloat data[6];
    struct {
        ELFloat minX;
        ELFloat maxX;
        ELFloat minY;
        ELFloat maxY;
        ELFloat minZ;
        ELFloat maxZ;
    };
};

typedef union _AABBBox AABBBox;

class ELGeometryVertexBuffer {
public:
    bool supportColorAttrib;
public:
    ELGeometryVertexBuffer();
    ~ELGeometryVertexBuffer();
    void enableColorAttrib();
    void append(ELGeometryVertex vertex);
    void append(ELGeometryColorVertex vertex);
    void append(ELGeometryTriangle triangle, ELInt matID = 0);
    void append(ELGeometryRect rect);
    void append(ELGeometryColorRect colorRect);
    ELSize rawLength();
    void caculatePerVertexNormal();
    void * data();
    void clear();
    ELVector3 size();

    GLuint getVBO();
private:
    int vbo;
    ELInt bufferLen;
    ELInt index;
    ELGeometryVertex *vertices;
    ELGeometryColorVertex *colorVertices;
    AABBBox aabbBox;
    void caculateTangents(ELVector3 *pTangent, ELVector3 *pBitangent, ELVector3 position, ELVector3 normal);
};


#endif //EZGL_ELGEOMETRYVERTEXBUFFER_H
