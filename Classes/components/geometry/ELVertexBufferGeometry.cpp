//
// Created by wangyang on 16/12/21.
//

#include "ELVertexBufferGeometry.h"

ELVertexBufferGeometry::ELVertexBufferGeometry() {
    smooth = false;
    vertexBuffer = new ELGeometryVertexBuffer();
}

ELVertexBufferGeometry::ELVertexBufferGeometry(bool smooth) : smooth(smooth) {
    vertexBuffer = new ELGeometryVertexBuffer();
}

ELVertexBufferGeometry::~ELVertexBufferGeometry() {
    delete vertexBuffer;
}

ELGeometryData ELVertexBufferGeometry::generateData() {
    vertexBuffer->clear();
    fillVertexBuffer(vertexBuffer);
    if (smooth) {
        vertexBuffer->caculatePerVertexNormal();
    }

    GLfloat *vertex = (GLfloat *)vertexBuffer->data();
    ELGeometryData data;
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer->rawLength(), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryVertex);
    data.vertexStride = sizeof(ELGeometryVertex);
    data.supportIndiceVBO = false;

    return data;
}

void ELVertexBufferGeometry::render() {
    ELGeometry::render();
}