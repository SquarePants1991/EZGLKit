//
// Created by wangyang on 16/12/9.
//

#include "ELWaterPlane.h"
#include "core/ELConfig.h"
#include "core/ELGameObject.h"

ELWaterPlane::ELWaterPlane() {
    createFramebuffers();
}

ELWaterPlane::ELWaterPlane(ELVector2 size) : size(size) {
    createFramebuffers();
}

ELVector4 ELWaterPlane::plane() {
    return ELVector4Make(0,1,0,gameObject()->transform->position.y);
}

ELVector4 ELWaterPlane::inversePlane() {
    return ELVector4Make(0,-1,0,gameObject()->transform->position.y);
}

void ELWaterPlane::render() {
    ELGeometry::render();
}

void ELWaterPlane::effectDidActive(ELEffect * effect) {
    glUniform1i(effect->program->uniform("dudvMap"), 13);
    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, dudvMap);

    glUniform1i(effect->program->uniform("reflectionMap"), 14);
    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, reflectionMap);

    glUniform1i(effect->program->uniform("refractionMap"), 15);
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, refractionMap);
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
    glad_glViewport(0,0,ELConfig::reflectionMapWidth,ELConfig::reflectionMapHeight);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_PLANE0);
    glad_glClearColor(1.0,1.0,1.0,1.0);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ELWaterPlane::endGenReflectionMap() {
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CLIP_PLANE0);
}

void ELWaterPlane::beginGenRefractionMap() {
    glad_glViewport(0,0,ELConfig::refractionMapWidth,ELConfig::refractionMapHeight);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_PLANE0);
    glad_glClearColor(1.0,1.0,1.0,1.0);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ELWaterPlane::endGenRefractionMap() {
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CLIP_PLANE0);
}

void ELWaterPlane::createFramebuffers() {
    genColorFramebuffer(reflectionFramebuffer, reflectionMap, ELConfig::reflectionMapWidth, ELConfig::reflectionMapHeight);
    genColorFramebuffer(refractionFramebuffer, refractionMap, ELConfig::refractionMapWidth, ELConfig::refractionMapHeight);
}

void ELWaterPlane::genColorFramebuffer(GLuint &outFramebuffer, GLuint &outTexture, ELUint width, ELUint height) {
    GLuint framebuffer;
    GLuint shadowTexture;

    glad_glGenFramebuffers(1, &framebuffer);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glad_glGenTextures(1, &shadowTexture);
    glad_glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);

    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    outFramebuffer = framebuffer;
    outTexture = shadowTexture;
}