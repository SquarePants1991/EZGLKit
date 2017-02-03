//
// Created by wang yang on 2016/11/27.
//

#include <core/ELGameObject.h>
#include "ELCubeGeometry.h"


ELCubeGeometry::ELCubeGeometry(ELVector3 size) : size(size) {
}

ELCubeGeometry::ELCubeGeometry(ELVector3 size, bool smooth) : ELVertexBufferGeometry(smooth), size(size) {

}

void ELCubeGeometry::fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) {
    // 柱子
    int segments = 4;
    float uvFactor = 80.0;
    // x 轴上的面
    ELGeometryRect rectXLeft = {
            {-size.x / 2, size.y / 2, size.z / 2},
            {-size.x / 2, -size.y / 2, size.z / 2},
            {-size.x / 2, -size.y / 2, -size.z / 2},
            {-size.x / 2, size.y / 2, -size.z / 2},
            {0, 0},
            {0, size.y / uvFactor},
            {size.z / uvFactor, size.y / uvFactor},
            {size.z / uvFactor, 0},
    };

    ELGeometryRect rectXRight = {
            {size.x / 2, size.y / 2, -size.z / 2},
            {size.x / 2, -size.y / 2, -size.z / 2},
            {size.x / 2, -size.y / 2, size.z / 2},
            {size.x / 2, size.y / 2, size.z / 2},
            {0, 0},
            {0, size.y / uvFactor},
            {size.z / uvFactor, size.y / uvFactor},
            {size.z / uvFactor, 0},
    };
    vertexBuffer->append(rectXLeft);
    vertexBuffer->append(rectXRight);

    // y 轴上的面
    ELGeometryRect rectYDown = {
            {-size.x / 2, -size.y / 2, -size.z / 2},
            {-size.x / 2, -size.y / 2, size.z / 2},
            {size.x / 2, -size.y / 2, size.z / 2},
            {size.x / 2, -size.y / 2, -size.z / 2},
            {0, 0},
            {0, size.z / uvFactor},
            {size.x / uvFactor, size.z / uvFactor},
            {size.x / uvFactor, 0},
    };

    ELGeometryRect rectYUp = {
            {size.x / 2, size.y / 2, -size.z / 2},
            {size.x / 2, size.y / 2, size.z / 2},
            {-size.x / 2, size.y / 2, size.z / 2},
            {-size.x / 2, size.y / 2, -size.z / 2},
            {0, 0},
            {0, size.z / uvFactor},
            {size.x / uvFactor, size.z / uvFactor},
            {size.x / uvFactor, 0},
    };
    vertexBuffer->append(rectYDown);
    vertexBuffer->append(rectYUp);

    // z 轴上的面
    ELGeometryRect rectZFar = {
            {-size.x / 2, size.y / 2, -size.z / 2},
            {-size.x / 2, -size.y / 2, -size.z / 2},
            {size.x / 2, -size.y / 2, -size.z / 2},
            {size.x / 2, size.y / 2, -size.z / 2},
            {0, 0},
            {0, size.y / uvFactor},
            {size.x / uvFactor, size.y / uvFactor},
            {size.x / uvFactor, 0},
    };

    ELGeometryRect rectZNear = {
            {size.x / 2, size.y / 2, size.z / 2},
            {size.x / 2, -size.y / 2, size.z / 2},
            {-size.x / 2, -size.y / 2, size.z / 2},
            {-size.x / 2, size.y / 2, size.z / 2},
            {0, 0},
            {0, size.y / uvFactor},
            {size.x / uvFactor, size.y / uvFactor},
            {size.x / uvFactor, 0},
    };

    vertexBuffer->append(rectZFar);
    vertexBuffer->append(rectZNear);
}
