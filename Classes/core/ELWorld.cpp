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
    ELVector3 eye = {0, 3, 15};
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


    activeEffect("default");
    ELGeometry::renderShadow = true;
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i));
        if (light != NULL) {
            genShadowTextureFromLight(light);
        }
    }

    ELGeometry::renderShadow =   false;
    ELGeometry::shadowMap = shadowTexture;

    activeEffect("shadow");
    glad_glViewport(0,0,fbWidth,fbHeight);
    glad_glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mainCamera = originMainCamera;
    ELNode::render();
}

void ELWorld::activeEffect(std::string effectName) {
    for (int i = 0; i < children.size(); ++i) {
        ELNode *node = children.at(i);
        ELEffect *effect = dynamic_cast<ELEffect *>(node);
        if (effect != NULL && effect->identity == effectName) {
            activedEffect = effect;
        }
    }
}

void ELWorld::genShadowTextureFromLight(ELLight *light) {
    glad_glViewport(0,0,1024,1024);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
    glad_glClear(GL_DEPTH_BUFFER_BIT);
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 0, 1};
    mainCamera = new ELCamera(light->position, center,up,60,1,mainCamera->nearZ,mainCamera->farZ);
    mainCamera->asOrtho(-50.0, 50.0, 50, -50, -40,40);
    ELNode::render();
    ELGeometry::lightCamera = mainCamera;
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ELWorld::createShadowFramebuffer() {
    GLuint framebuffer;
    GLuint renderbuffer;
    GLuint shadowTexture;

    glad_glGenFramebuffers(1, &framebuffer);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenRenderbuffers(1, &renderbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    glad_glGenTextures(1, &shadowTexture);
    glad_glBindTexture(GL_TEXTURE_2D, shadowTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);

//    glad_glDrawBuffer(GL_NONE);
//    glad_glReadBuffer(GL_NONE);

    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->shadowFramebuffer = framebuffer;
    this->shadowTexture = shadowTexture;
}