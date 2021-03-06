//
// Created by wangyang on 16/12/2.
//

#ifndef EZGL_FGSCENE_H
#define EZGL_FGSCENE_H

#include "EZGL.h"

class FGScene {
public:
    FGScene(std::shared_ptr<ELWorld> world);
    ELRigidBody *playerRigidBody;
    void update(ELFloat timeInSecs);

    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
private:
    prop_strong(ELWorld, world);
    void createMonkey();
    void createPlane();
    void createFloor();
    void createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);
};


#endif //EZGL_FGSCENE_H
