//
// Created by wangyang on 16/12/2.
//

#ifndef EZGL_FGSCENE_H
#define EZGL_FGSCENE_H

#include "EZGL.h"

class FGScene {
public:
    FGScene(ELWorld *world);
    ELRigidBody *playerRigidBody;
    ELLight * defaultLight;

    void update(ELFloat timeInSecs);

    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
private:
    ELWorld *world;
    void createScene();

    void createSkySphere();
    void createBoundWall(ELVector3 offset,ELFloat width,ELFloat height,ELFloat wallHeight = 3.5);
    void createFloor();
    void createMonkey();
    void createMiddleWalls(ELVector3 offset,ELFloat width,ELFloat height);
    void createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder = false, int collisionGroup=1, int collisionMask=1, ELVector3 velocity=ELVector3Make(0,0,0));
    void createBoardGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap);
    void createParticalGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap);
    void createWater();
    void createTerrain();
    void createSphereGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);
};


#endif //EZGL_FGSCENE_H
