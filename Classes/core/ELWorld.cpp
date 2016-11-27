//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "EZGLBase.h"

ELWorld::ELWorld() {

}

ELWorld::ELWorld(ELFloat aspect) {
    ELVector3 eye = {0, 2, 5};
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 1, 0};
    mainCamera = new ELCamera(eye, center, up, 60.0, aspect, 1, 1000);
    addNode(mainCamera);
}

void ELWorld::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    ELNode::render();
}