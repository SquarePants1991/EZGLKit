//
//  EL2DPlane.cpp
//  EZGLSDK
//
//  Created by wangyang on 2017/3/17.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#include "EL2DPlane.h"
#include "../../core/ELGameObject.h"

EL2DPlane::EL2DPlane(ELVector2 size) : size(size) {
    
}

EL2DPlane::~EL2DPlane() {
    delete vertexBuffer;
}

ELGeometryData EL2DPlane::generateData() {
    vertexBuffer = new ELGeometryVertexBuffer();
    ELGeometryRect rect = {
        {0.5f * size.x  ,  0.5f * size.y  , -100.0f  },
        {0.5f * size.x  ,  -0.5f * size.y , -100.0f  },
        {-0.5f * size.x , -0.5f * size.y  , -100.0f },
        {-0.5f * size.x , 0.5f * size.y   , -100.0f },
        {1, 0},
        {0, 0},
        {0, 1},
        {1, 1},
        
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

void EL2DPlane::render() {
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    std::string oldEffect = this->gameObject()->world.lock()->activedEffect->identity;
    std::string oldCamera = this->gameObject()->world.lock()->activedCamera->identity;
    this->gameObject()->world.lock()->activeEffect("disp_video");
    this->gameObject()->world.lock()->activeCamera("ortho");
    ELGeometry::render();
    this->gameObject()->world.lock()->activeCamera(oldCamera);
    this->gameObject()->world.lock()->activeEffect(oldEffect);
    glDepthMask(GL_TRUE);
}

void EL2DPlane::update(ELFloat timeInSecs) {
    ELGeometry::update(timeInSecs);
    
}
