//
// Created by wangyang on 16/12/28.
//

#include "ELRenderPass.h"
#include "ELWorld.h"

void ELRenderPass::render(ELWorld *world) {
    world->orderedRender();
}