//
// Created by wangyang on 16/11/25.
//

#include "ELPlaneGeometry.h"

ELPlaneGeometry::ELPlaneGeometry(ELVector2 size) : size(size) {
    smooth = false;
}

ELPlaneGeometry::ELPlaneGeometry(ELVector2 size, bool smooth) : ELVertexBufferGeometry(smooth), size(size) {

}

void ELPlaneGeometry::fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) {
    ELGeometryRect rect = {
            {0.5f * size.x , 0.0f ,  -0.5f * size.y  },
            {0.5f * size.x , 0.0f ,  0.5f * size.y   },
            {-0.5f * size.x, 0.0f , 0.5f * size.y   },
            {-0.5f * size.x, 0.0f , -0.5f * size.y  },
            {0, 0},
            {0, size.y / 8},
            {size.x / 8, size.y / 8},
            {size.x / 8, 0}
    };
    vertexBuffer->append(rect);
}