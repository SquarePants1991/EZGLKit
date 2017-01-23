//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "ELLight.h"
#include "ELGameObject.h"
#include "utils/ELAssets.h"
#include "utils/ELGLState.h"
#include "ELRenderPass.h"

#define UseDepthFramebuffer 1

ELWorld::ELWorld() {
    glEnable(GL_DEPTH_TEST);

    ELGLState::setup();
    ELGLState::set(GL_CULL_FACE_MODE, GL_BACK);
    ELAssets::shared()->addSearchPath("assets/shaders");
    ELAssets::shared()->addSearchPath("assets/textures");
}

void ELWorld::enablePhysics() {
    physicsWorld = ELPhysicsWorld::shared();
    addNode(physicsWorld);
}

void ELWorld::enableDefaultCamera(ELFloat aspect) {
    ELVector3 eye = {0, 200, 500};
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 1, 0};
    ELCamera * defaultCamera = new ELCamera(eye, center, up, 70.0, aspect, 0.1, 1000);
    defaultCamera->identity = "default";
    addNode(defaultCamera);
    activeCamera("default");
}

void ELWorld::update(ELFloat timeInSecs) {
    activeCamera("main");
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    for (std::vector<ELRenderPass *>::iterator iter = renderPasses.begin(); iter != renderPasses.end(); ++iter) {
        (*iter)->render(this);
    }
    renderScene();
}

void ELWorld::renderScene() {
    activeEffect("render_scene");
    activeCamera("main");
//    glViewport(0,0,fbWidth,fbHeight);
    glClearColor(0.6f, 0.6, 0.6, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activedEffect->prepare();
    glUniform1i(activedEffect->program->uniform("useAdditionMatrix"),0);
    glUniform1i(activedEffect->program->uniform("enableClipPlane0"), 0);
    //bind shadow texture to 6 ~ 10
    int lightIndex = 0;
    char shadowMapUniformName[512];
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i));
        if (light != NULL && light->isShadowEnabled) {
            GLuint channel = GL_TEXTURE6 + lightIndex;
            glActiveTexture(channel);
            glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
            snprintf(shadowMapUniformName,512,"shadowMap[%d]",lightIndex);
            glUniform1i(activedEffect->program->uniform(shadowMapUniformName), channel - GL_TEXTURE0);
            snprintf(shadowMapUniformName,512,"lightViewProjection[%d]",lightIndex);
            glUniformMatrix4fv(activedEffect->program->uniform(shadowMapUniformName), 1, 0, (GLfloat *)light->shadowMapGenCamera()->matrix().m);
        }
    }

    orderedRender();
}

void ELWorld::orderedRender() {
    ELNode::drawTransparency  = false;
    ELNode::render();
    ELNode::drawTransparency  = true;
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

void ELWorld::addRenderPass(ELRenderPass *renderPass) {
    renderPasses.push_back(renderPass);
}
