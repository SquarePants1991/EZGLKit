//
// Created by wang yang on 2016/12/3.
//

#include "ELBoard.h"
#include "core/ELGameObject.h"

ELBoard::ELBoard(ELVector2 size) : size(size) {

}

ELBoard::~ELBoard() {
    delete vertexBuffer;
}

ELGeometryData ELBoard::generateData() {
    vertexBuffer = new ELGeometryVertexBuffer();
    ELGeometryRect rect = {
            {0.5f * size.x  ,  0.5f * size.y  , 0.0f  },
            {0.5f * size.x  ,  -0.5f * size.y , 0.0f  },
            {-0.5f * size.x , -0.5f * size.y  , 0.0f },
            {-0.5f * size.x , 0.5f * size.y   , 0.0f },
            {0, 0},
            {0, 1},
            {1, 1},
            {1, 0}
    };
    vertexBuffer->append(rect);

//    vertexBuffer->caculatePerVertexNormal();

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

void ELBoard::render() {
    glDisable(GL_CULL_FACE);
    ELGeometry::render();
    glEnable(GL_CULL_FACE);
}

void ELBoard::update(ELFloat timeInSecs) {
    ELGeometry::update(timeInSecs);
    ELVector3 cameraPosition = gameObject()->mainCamera()->position();
    ELFloat cx = cameraPosition.x - gameObject()->transform->position.x;
    ELFloat cz = cameraPosition.z - gameObject()->transform->position.z;

    ELFloat angle = 0;
    if (cz == 0) {
        angle = 0;
    } else {
        ELFloat tanVal = cx / cz;
        angle = atan(tanVal);
    }
    gameObject()->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(angle,0,1,0);
}