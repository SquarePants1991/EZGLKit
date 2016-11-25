//
// Created by wangyang on 16/11/25.
//

#include "ELPlanGeometry.h"

ELPlanGeometry::ELPlanGeometry(ELVector2 size) : size(size) {

}

ELPlanGeometry::~ELPlanGeometry() {
    delete vertexBuffer;
}

ELGeometryData ELPlanGeometry::generateData() {
    vertexBuffer = new ELGeometryVertexBuffer();
    ELGeometryRect rect = {
            {0.5f * size.x, 0.0f, -0.5f * size.y},
            {0.5f * size.x, 0.0f, 0.5f * size.y},
            {-0.5f * size.x, 1.0f, 0.5f * size.y},
            {-0.5f * size.x, 1.0f, -0.5f * size.y},
            {0, 0},
            {0, 1},
            {1, 1},
            {1, 0}
    };
    vertexBuffer->append(rect);

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

void ELPlanGeometry::update(ELFloat timeInSecs) {

}

void ELPlanGeometry::render() {
    ELGeometry::render();
}