//
// Created by wangyang on 16/12/9.
//

#include "../utils/ELGLState.h"
#include "ELWaterPlane.h"
#include "../core/ELConfig.h"
#include "../core/ELGameObject.h"

bool ELWaterPlane::isInWaterPlanePreparePass = false;

ELWaterPlane::ELWaterPlane() {
    createFramebuffers();
    waterColor = ELVector4Make(1.0,1.0,0.0,1.0);
    this->kind = "water plane";
}

ELWaterPlane::ELWaterPlane(ELVector2 size) : size(size) {
    createFramebuffers();
    waterColor = ELVector4Make(0.0,0.6,0.4,1.0);
    this->kind = "water plane";
}

ELWaterPlane::~ELWaterPlane() {
    glDeleteFramebuffers(1, &reflectionFramebuffer);
    glDeleteFramebuffers(1, &refractionFramebuffer);
}

ELVector4 ELWaterPlane::plane() {
    return ELVector4Make(0,1,0,gameObject()->transform->position.y);
}

ELVector4 ELWaterPlane::inversePlane() {
    return ELVector4Make(0,-1,0,gameObject()->transform->position.y);
}

void ELWaterPlane::render() {
    if (ELWaterPlane::isInWaterPlanePreparePass) {
        return;
    } else {
        ELGLState::saveState();
        ELGLState::set(GL_CULL_FACE, GL_FALSE);
        ELGeometry::render();
        ELGLState::restoreState();
    }
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

    glUniform4fv(effect->program->uniform("waterBaseColor"), 1, (GLfloat *)waterColor.v);
}

void ELWaterPlane::effectDidInactive(ELEffect * effect) {

}

ELGeometryData ELWaterPlane::generateData() {
    vertexBuffer = retain_ptr_init(ELGeometryVertexBuffer);
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

void ELWaterPlane::beginGenReflectionMap() {
    glViewport(0,0,ELConfig::reflectionMapWidth,ELConfig::reflectionMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
#if Platform_OSX
//    glEnable(GL_CLIP_PLANE0);
#else
    glEnable(GL_CLIP_DISTANCE0_APPLE);
#endif
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ELWaterPlane::endGenReflectionMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#if Platform_OSX
//    glDisable(GL_CLIP_PLANE0);
#else
    glDisable(GL_CLIP_DISTANCE0_APPLE);
#endif
}

void ELWaterPlane::beginGenRefractionMap() {
    glViewport(0,0,ELConfig::refractionMapWidth,ELConfig::refractionMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer);
#if Platform_OSX
//    glEnable(GL_CLIP_PLANE0);
#else
    glEnable(GL_CLIP_DISTANCE0_APPLE);
#endif
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ELWaterPlane::endGenRefractionMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#if Platform_OSX
//    glDisable(GL_CLIP_PLANE0);
#else
    glDisable(GL_CLIP_DISTANCE0_APPLE);
#endif
}

void ELWaterPlane::createFramebuffers() {
    genColorFramebuffer(reflectionFramebuffer, reflectionMap, ELConfig::reflectionMapWidth, ELConfig::reflectionMapHeight);
    genColorFramebuffer(refractionFramebuffer, refractionMap, ELConfig::refractionMapWidth, ELConfig::refractionMapHeight);
}

void ELWaterPlane::genColorFramebuffer(GLuint &outFramebuffer, GLuint &outTexture, ELUint width, ELUint height) {
    GLuint framebuffer;
    GLuint shadowTexture;
    GLuint depthBuffer;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    outFramebuffer = framebuffer;
    outTexture = shadowTexture;
}
