//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "EZGLBase.h"
#include "ELLight.h"
#include "components/ELWaterPlane.h"

#define UseDepthFramebuffer 1

ELWorld::ELWorld() {
    physicsWorld = ELPhysicsWorld::shared();
    addNode(physicsWorld);
}

ELWorld::ELWorld(ELFloat aspect) {
    ELVector3 eye = {0, 1.7, 0};
    ELVector3 center = {0, 0, -10};
    ELVector3 up = {0, 1, 0};
    ELCamera * defaultCamera = new ELCamera(eye, center, up, 70.0, aspect, 0.1, 1000);
    defaultCamera->identity = "main";
    addNode(defaultCamera);

    physicsWorld = ELPhysicsWorld::shared();
    addNode(physicsWorld);

    activeCamera("main");
}

void ELWorld::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    renderShadowMaps();
    renderReflectionMaps();
    renderScene();
}

void ELWorld::renderShadowMaps() {
    activeEffect("gen_shadow");
    activedEffect->prepare();
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i));
        if (light != NULL) {
            light->beginGenShadowMap();
            activeCamera(light->identity + "-shadow-camera", light->shadowMapGenCamera());
            ELNode::render();
            light->endGenShadowMap();
        }
    }
}

void ELWorld::renderReflectionMaps() {
    activeEffect("render_scene");
    activedEffect->prepare();
    activeCamera("main");
    std::vector<ELNode *> waterPlanes = findChildrenWithKind("water_plane", true);
    for (int i = 0; i < waterPlanes.size(); ++i) {
        ELWaterPlane * waterPlane = dynamic_cast<ELWaterPlane *>(waterPlanes.at(i));
        if (waterPlane != NULL) {
            waterPlane->beginGenReflectionMap();
            activedCamera->flip(true);
            ELNode::render();
            waterPlane->endGenReflectionMap();
            activedCamera->flip(false);
        }
    }
}

void ELWorld::renderScene() {
    activeEffect("render_scene");
    activeCamera("main");
    glad_glViewport(0,0,fbWidth,fbHeight);
    glad_glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activedEffect->prepare();
    //bind shadow texture to 6 ~ 10
    int lightIndex = 0;
    char shadowMapUniformName[512];
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i));
        if (light != NULL && light->isShadowEnabled) {
            GLuint channel = GL_TEXTURE6 + lightIndex;
            glad_glActiveTexture(channel);
            glad_glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
            snprintf(shadowMapUniformName,512,"shadowMap[%d]",lightIndex);
            glad_glUniform1i(activedEffect->program->uniform(shadowMapUniformName), channel - GL_TEXTURE0);
            snprintf(shadowMapUniformName,512,"lightViewProjection[%d]",lightIndex);
            glad_glUniformMatrix4fv(activedEffect->program->uniform(shadowMapUniformName), 1, 0, light->shadowMapGenCamera()->matrix().m);
        }
    }

    ELNode::render();

    glad_glDisable(GL_CLIP_PLANE0);
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

void ELWorld::activeCamera(std::string cameraName, ELCamera *camera) {
    bool cameraFound = false;
    for (int i = 0; i < children.size(); ++i) {
        ELNode *node = children.at(i);
        ELCamera *camera = dynamic_cast<ELCamera *>(node);
        if (camera != NULL && camera->identity == cameraName) {
            activedCamera = camera;
            cameraFound = true;
        }
    }
    if (cameraFound == false && camera != NULL) {
        addNode(camera);
        activedCamera = camera;
    }
}