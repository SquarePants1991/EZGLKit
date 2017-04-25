//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELWORLD_H
#define EZGL_ELWORLD_H

#include "ELNode.h"
#include "ELCamera.h"
#include "ELPhysicsWorld.h"
#include "ELEffect.h"

#include <vector>

class ELLight;
class ELRenderPass;

class ELWorld : public ELNode {
public:
    ELCamera * activedCamera;
    ELEffect * activedEffect;
    ELPhysicsWorld * physicsWorld;

    int fbWidth;
    int fbHeight;
public:
    ELWorld();
    ~ELWorld();

    virtual void update(ELFloat timeInSecs);
    virtual void render();

    void orderedRender();
    void enablePhysics();
    void enableDefaultCamera(ELFloat aspect);
    void activeEffect(std::string effectName);
    void activeCamera(std::string cameraName, ELCamera *camera = NULL);

    void addRenderPass(ELRenderPass *renderPass);
    void setViewport(ELInt left, ELInt bottom, ELInt width, ELInt height);
private:
    std::vector<std::shared_ptr<ELRenderPass> > renderPasses;
    
    std::shared_ptr<ELVector4 *> viewport;
private:
    void renderShadowMaps();
    void renderReflectionMaps();
    void renderRefractionMaps();
    void renderScene();
};


#endif //EZGL_ELWORLD_H
