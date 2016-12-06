//
// Created by wangyang on 16/12/6.
//

#include "ELParticalSystem.h"
#include "ELPartical.h"
#include "core/ELGameObject.h"
#include "ELGeometry.h"

ELParticalSystem::ELParticalSystem() : isDataInitialized(false) {

}

ELGeometryData ELParticalSystem::generateData() {
    if (isDataInitialized == false) {
        vertexBuffer = new ELGeometryVertexBuffer();
        partical = new ELPartical();
    } else {
        vertexBuffer->clear();
    }
    ELGeometryData data;

    partical->size = ELVector2Make(2,2);
    partical->update(gameObject()->mainCamera()->position(),0,ELVector3Make(0,0,0));
    vertexBuffer->append(partical->quadRect);

    GLfloat *vertex = (GLfloat *)(vertexBuffer->data());
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer->rawLength(), vertex, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryVertex);
    data.vertexStride = sizeof(ELGeometryVertex);
    data.supportIndiceVBO = false;
    currentData = data;
    isDataInitialized = true;
    return data;
}

void ELParticalSystem::update(ELFloat timeInSecs) {
    ELGeometry::update(timeInSecs);
}

void ELParticalSystem::render() {
    this->setNeedRegenData();
    ELGeometry::render();
}