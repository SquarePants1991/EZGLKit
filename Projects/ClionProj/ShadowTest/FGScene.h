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
    ELLight * defaultLight;

    void update(ELFloat timeInSecs);

    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
private:
    prop_strong(ELWorld, world);
    void createScene();

    void createPlayer();
    void createFloor();
    ELGeometry * createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,ELVector4 diffuseColor,GLuint normalMap, bool hasBorder = false, int collisionGroup=1, int collisionMask=1, ELVector3 velocity=ELVector3Make(0,0,0));
    void createSphereGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);
    void createConeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);
    void createTerrain();
    void createMonkey();
};


#endif //EZGL_FGSCENE_H
