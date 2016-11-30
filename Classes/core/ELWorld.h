//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELWORLD_H
#define EZGL_ELWORLD_H

#include "ELNode.h"
#include <vector>
#include "ELCamera.h"
#include "ELPhysicsWorld.h"

class ELLight;

class ELWorld : public ELNode {
public:
    ELCamera *mainCamera;
    ELPhysicsWorld *physicsWorld;
    ELUint shadowTexture;
    ELUint originFramebuffer;

    int fbWidth;
    int fbHeight;
public:
    ELWorld();
    ELWorld(ELFloat aspect);
    virtual void update(ELFloat timeInSecs);
    virtual void render();

private:
    ELUint shadowFramebuffer;
    void genShadowTextureFromLight(ELLight *light);
    void createShadowFramebuffer();
};


#endif //EZGL_ELWORLD_H
