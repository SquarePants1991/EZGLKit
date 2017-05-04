//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "ELLight.h"
#include "ELGameObject.h"
#include "../utils/ELAssets.h"
#include "../utils/ELGLState.h"
#include "../core/ELTexture.h"
#include "ELRenderPass.h"

#define UseDepthFramebuffer 1

ELWorld::ELWorld() :
    defaultFBO(0)
{
    glEnable(GL_DEPTH_TEST);

    ELGLState::setup();
    ELGLState::set(GL_CULL_FACE_MODE, GL_BACK);
    ELAssets::shared()->addSearchPath("assets/shaders");
    ELAssets::shared()->addSearchPath("assets/textures");

    kind = "world";
}

ELWorld::~ELWorld() {

}

void ELWorld::enablePhysics() {
    physicsWorld = new ELPhysicsWorld();
    addNode(retain_ptr(ELPhysicsWorld, physicsWorld));
}

void ELWorld::enableDefaultCamera(ELFloat aspect) {
    ELVector3 eye = {0, 10, 10};
    ELVector3 center = {0, 10, 0};
    ELVector3 up = {0, 1, 0};
    ELCamera * defaultCamera = new ELCamera(eye, center, up, 70.0, aspect, 0.1, 1000);
    defaultCamera->identity = "main";
    addNode(std::shared_ptr<ELCamera>(defaultCamera));
    activeCamera("main");
}

void ELWorld::update(ELFloat timeInSecs) {
    activeCamera("main");
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter) {
        (*iter)->render(this);
    }
    renderScene();
}

void ELWorld::renderScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    activeEffect("render_scene");
    activeCamera("main");
    if (viewport.get() != NULL) {
        glViewport((*viewport)->x, (*viewport)->y, (*viewport)->z, (*viewport)->w);
    } else if (fbWidth > 0 && fbHeight > 0){
        glViewport(0,0,fbWidth, fbHeight);
    }
    glClearColor(0.6f, 0.6, 0.6, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activedEffect->prepare();
    glUniform1i(activedEffect->program->uniform("useAdditionMatrix"),0);
    glUniform1i(activedEffect->program->uniform("enableClipPlane0"), 0);
    //bind shadow texture to 6 ~ 10
    int lightIndex = 0;
    char shadowMapUniformName[512];
//    GLuint texture =  ELTexture::texture(ELAssets::shared()->findFile("island.png"))->value;
    for (int i = 0; i < children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(children.at(i).get());
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
        ELNode *node = children.at(i).get();
        ELEffect *effect = dynamic_cast<ELEffect *>(node);
        if (effect != NULL && effect->identity == effectName) {
            activedEffect = effect;
        }
    }
}

void ELWorld::activeCamera(std::string cameraName, ELCamera *camera) {
    bool cameraFound = false;
    for (int i = 0; i < children.size(); ++i) {
        ELNode *node = children.at(i).get();
        ELCamera *camera = dynamic_cast<ELCamera *>(node);
        if (camera != NULL && camera->identity == cameraName) {
            activedCamera = camera;
            cameraFound = true;
            return;
        }
    }
    if (cameraFound == false && camera != NULL) {
        addNode(std::shared_ptr<ELCamera>(camera));
        activedCamera = camera;
    }
}

void ELWorld::addRenderPass(ELRenderPass *renderPass) {
    renderPasses.push_back(retain_ptr(ELRenderPass, renderPass));
}

void ELWorld::setViewport(ELInt left, ELInt bottom, ELInt width, ELInt height) {
    ELVector4 * pViewport = new ELVector4();
    pViewport->x = left;
    pViewport->y = bottom;
    pViewport->z = width;
    pViewport->w = height;
    viewport = std::make_shared<ELVector4 *>(pViewport);
}
