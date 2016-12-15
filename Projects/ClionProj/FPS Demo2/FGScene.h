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
private:
    ELWorld *world;
    void createScene();

    void createBoundWall(ELVector3 offset,ELFloat width,ELFloat height,ELFloat wallHeight = 3.5);
    void createFloor();
    void createMonkey();
    void createMiddleWalls(ELVector3 offset,ELFloat width,ELFloat height);
    void createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap);
    void createBoardGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap);
    void createParticalGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap);
    void createWater();
    void createTerrain();
};


#endif //EZGL_FGSCENE_H
