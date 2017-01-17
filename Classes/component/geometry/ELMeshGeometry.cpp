//
// Created by wangyang on 16/11/28.
//

#include <core/ELGameObject.h>
#include "ELMeshGeometry.h"

ELMeshGeometry::ELMeshGeometry() {

}

ELMeshGeometry::ELMeshGeometry(ELGeometryData data) : _data(data) {

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
}

void ELMeshGeometry::fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) {
    if (vertexBufferProvider != NULL) {
        vertexBufferProvider->update(0,vertexBuffer);
    }
}
