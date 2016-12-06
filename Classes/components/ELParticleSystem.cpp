//
// Created by wangyang on 16/12/6.
//

#include "ELParticalSystem.h"
#include "ELPartical.h"
#include "core/ELGameObject.h"
#include "ELGeometry.h"

ELParticalSystem::ELParticalSystem() : isDataInitialized(false), partical(NULL) {

}

ELGeometryData ELParticalSystem::generateData() {
    if (isDataInitialized == false) {
        vertexBuffer = new ELGeometryVertexBuffer();
        partical = new ELPartical();
    } else {
        vertexBuffer->clear();
    }
    ELGeometryData data;

    partical->size = ELVector2Make(0.3,0.3);
    partical->transform = *(gameObject()->transform);
    partical->velocity = ELVector3Make(0,10,0);

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
    if (partical != NULL) {
        partical->update(gameObject()->mainCamera()->position(), timeInSecs, ELVector3Make(0, -0.2, 0));
    }
    ELGeometry::update(timeInSecs);
}

void ELParticalSystem::render() {
    this->setNeedRegenData();
    glDisable(GL_CULL_FACE);
    ELGeometry::render();
    glEnable(GL_CULL_FACE);
}