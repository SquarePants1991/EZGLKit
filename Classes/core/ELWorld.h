//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELWORLD_H
#define EZGL_ELWORLD_H

#include "ELNode.h"
#include <vector>
#include "ELCamera.h"
#include "ELPhysicsWorld.h"
#include "ELEffect.h"

class ELLight;

class ELWorld : public ELNode {
public:
    ELCamera *activedCamera;
    ELEffect *activedEffect;
    ELPhysicsWorld *physicsWorld;

    int fbWidth;
    int fbHeight;
public:
    ELWorld();
    ELWorld(ELFloat aspect);
    virtual void update(ELFloat timeInSecs);
    virtual void render();

    void activeEffect(std::string effectName);
    void activeCamera(std::string cameraName, ELCamera *camera = NULL);
private:

    void renderShadowMaps();
    void renderReflectionMaps();
    void renderRefractionMaps();
    void renderScene();
};


#endif //EZGL_ELWORLD_H
