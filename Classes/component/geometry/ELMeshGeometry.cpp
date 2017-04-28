//
// Created by wangyang on 16/11/28.
//

#include "../../core/ELGameObject.h"
#include "ELMeshGeometry.h"

ELMeshGeometry::ELMeshGeometry() {

}

ELMeshGeometry::ELMeshGeometry(ELGeometryData data) :
        _data(data),
        currentTime(0),
        currentAnimation(ELAnimation::None())
{

}

void ELMeshGeometry::setAnimation(std::string animationName) {
    if(animations.size() == 0) {
        currentAnimation = ELAnimation::Default();
        return;
    }
    auto animationIndex = animations.find(animationName);
    if (animationIndex != animations.end()) {
        currentAnimation = animations[animationName];
        currentTime = currentAnimation.startTime / 1000.0;
    }
}

ELGeometryData ELMeshGeometry::generateData() {
    if (vertexBufferProvider == NULL) {
        return _data;
    } else {
        return ELVertexBufferGeometry::generateData();
    }
}

void ELMeshGeometry::update(ELFloat timeInSecs) {
    ELGeometry::update(timeInSecs);
    if (currentAnimation != ELAnimation::None() &&
        currentAnimation != ELAnimation::Default()) {
        isGeometryDataValid = false;
        currentTime += timeInSecs;
        if (currentTime >= currentAnimation.stopTime / 1000.0f) {
            currentTime = currentAnimation.startTime / 1000.0f;
        }
    }
}

void ELMeshGeometry::fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) {
    if (currentAnimation != ELAnimation::None()) {
        if (vertexBufferProvider != NULL) {
            vertexBufferProvider->update(currentAnimation.name.c_str(), currentTime, vertexBuffer, this->clustersMatrix);
        }
    }
}

void ELMeshGeometry::effectDidActive(ELEffect * effect) {
    GLuint clustersLocation = (GLuint)effect->program->uniform((char *)"clusters");
    glUniformMatrix4fv(clustersLocation, (GLuint)clustersMatrix.size(), false, (GLfloat *)(clustersMatrix.data()));
}
