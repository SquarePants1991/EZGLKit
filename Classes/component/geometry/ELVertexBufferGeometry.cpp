//
// Created by wangyang on 16/12/21.
//


#include "core/ELEffect.h"
#include "component/ELGeometry.h"
#include "ELVertexBufferGeometry.h"

ELVertexBufferGeometry::ELVertexBufferGeometry() :
        vertexBufferProvider(NULL)
{
    smooth = false;
    vertexBuffer = new ELGeometryVertexBuffer();
}

ELVertexBufferGeometry::ELVertexBufferGeometry(bool smooth) :
        vertexBufferProvider(NULL),
        smooth(smooth)
{
    vertexBuffer = new ELGeometryVertexBuffer();
}

ELVertexBufferGeometry::~ELVertexBufferGeometry() {
    if (vertexBuffer != NULL) {
        delete vertexBuffer;
    }
}

ELGeometryData ELVertexBufferGeometry::generateData() {
    vertexBuffer->clear();
    fillVertexBuffer(vertexBuffer);
    if (smooth) {
        vertexBuffer->caculatePerVertexNormal();
    }
    size = vertexBuffer->size();

    ELGeometryData data;
    data.vertexVBO = vertexBuffer->getVBO();
    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryVertex);
    data.vertexStride = sizeof(ELGeometryVertex);
    data.supportIndiceVBO = false;
    data.supportColorAttrib = false;

    return data;
}
