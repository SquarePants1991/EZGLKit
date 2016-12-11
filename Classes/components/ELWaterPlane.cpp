//
// Created by wangyang on 16/12/9.
//

#include "ELWaterPlane.h"
#include "core/ELConfig.h"

ELWaterPlane::ELWaterPlane() {
    createReflectionFramebuffer();
}

ELWaterPlane::ELWaterPlane(ELVector2 size) : size(size) {
    createReflectionFramebuffer();
}

ELVector3 ELWaterPlane::plane() {
    return ELVector3Make(0,1,0);
}

void ELWaterPlane::render() {
    ELGeometry::render();
}

ELGeometryData ELWaterPlane::generateData() {
    vertexBuffer = new ELGeometryVertexBuffer();
    ELGeometryRect rect = {
            {0.5f * size.x , 0.0f ,  -0.5f * size.y  },
            {0.5f * size.x , 0.0f ,  0.5f * size.y   },
            {-0.5f * size.x, 0.0f , 0.5f * size.y   },
            {-0.5f * size.x, 0.0f , -0.5f * size.y  },
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

std::string ELWaterPlane::kind() {
    return "water_plane";
}

void ELWaterPlane::beginGenReflectionMap() {
    glad_glViewport(0,0,ELConfig::shadowMapWidth,ELConfig::shadowMapHeight);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_PLANE0);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ELWaterPlane::endGenReflectionMap() {
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CLIP_PLANE0);
    material.diffuseMap = reflectionMap;
    material.ambientMap = reflectionMap;
}

void ELWaterPlane::createReflectionFramebuffer() {
    genColorFramebuffer();
}

void ELWaterPlane::genDepthFramebuffer() {
    GLuint framebuffer;
    GLuint reflectTexture;

    glad_glGenFramebuffers(1, &framebuffer);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glad_glGenTextures(1, &reflectTexture);
    glad_glBindTexture(GL_TEXTURE_2D, reflectTexture);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ELConfig::shadowMapWidth, ELConfig::shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, reflectTexture, 0);
    glad_glDrawBuffer(GL_NONE);
    glad_glReadBuffer(GL_NONE);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->reflectionFramebuffer = framebuffer;
    this->reflectionMap = reflectTexture;
}

void ELWaterPlane::genColorFramebuffer() {
    GLuint framebuffer;
    GLuint shadowTexture;

    glad_glGenFramebuffers(1, &framebuffer);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glad_glGenTextures(1, &shadowTexture);
    glad_glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ELConfig::shadowMapWidth, ELConfig::shadowMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);

    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->reflectionFramebuffer = framebuffer;
    this->reflectionMap = shadowTexture;
}