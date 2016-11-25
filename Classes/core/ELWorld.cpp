//
// Created by wangyang on 16/11/24.
//

#include "ELWorld.h"
#include "EZGLBase.h"

void ELWorld::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
}

void ELWorld::render() {
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ELNode::render();
}