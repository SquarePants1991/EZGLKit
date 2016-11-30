//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "EZGLBase.h"
#include "ELLight.h"
#include "components/ELGeometry.h"

ELWorld::ELWorld() {
    physicsWorld = ELPhysicsWorld::shared();
    addNode(physicsWorld);

    createShadowFramebuffer();
}

ELWorld::ELWorld(ELFloat aspect) {
    ELVector3 eye = {0, 2, 5};
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 1, 0};
    mainCamera = new ELCamera(eye, center, up, 60.0, aspect, 1, 1000);
    addNode(mainCamera);

    physicsWorld = ELPhysicsWorld::shared();
    addNode(physicsWorld);

    createShadowFramebuffer();
}

void ELWorld::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    ELCamera *originMainCamera = mainCamera;

    GLint drawFboId = 0, readFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);

    ELGeometry::renderShadow = true;
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i));
        if (light != NULL) {
            genShadowTextureFromLight(light);
        }
    }

    ELGeometry::renderShadow =   false;
    ELGeometry::shadowMap = shadowTexture;
    glBindFramebuffer(GL_FRAMEBUFFER, drawFboId);
    glViewport(0,0,fbWidth,fbHeight);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mainCamera = originMainCamera;
    ELNode::render();
}

void ELWorld::genShadowTextureFromLight(ELLight *light) {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);
    glViewport(0,0,1024,1024);
    glClear(GL_DEPTH_BUFFER_BIT);
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 0, 1};
    mainCamera = new ELCamera(light->position, center,up,60,1,mainCamera->nearZ,mainCamera->farZ);
    mainCamera->asOrtho(-10.0, 10.0, 10, -10, mainCamera->nearZ,mainCamera->farZ);
    ELNode::render();
    ELGeometry::lightCamera = mainCamera;
}

void ELWorld::createShadowFramebuffer() {
    GLuint framebuffer;
    GLuint renderbuffer;
    GLuint shadowTexture;

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &shadowTexture);
    glGenRenderbuffers(1, &renderbuffer);

//    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024, 1024);

    glBindTexture(GL_TEXTURE_2D, shadowTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->shadowFramebuffer = framebuffer;
    this->shadowTexture = shadowTexture;
}