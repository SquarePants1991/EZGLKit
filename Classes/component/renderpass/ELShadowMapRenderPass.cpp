//
// Created by wangyang on 16/12/28.
//

#include "ELShadowMapRenderPass.h"
#include "../../core/ELWorld.h"
#include "../../core/ELLight.h"
#include "../../utils/ELGLState.h"

void ELShadowMapRenderPass::render(ELWorld *world) {
    for (int i = 0; i < world->children.size(); ++i) {
        ELLight *light = dynamic_cast<ELLight *>(world->children.at(i).get());
        if (light != NULL && light->isShadowEnabled) {
            ELGLState::saveState();
            ELGLState::cullFace(GL_FRONT);
            world->activeEffect("gen_shadow");
            world->activedEffect->prepare();
            light->beginGenShadowMap();
            world->activeCamera(light->identity + "-shadow-camera", light->shadowMapGenCamera().get());
            world->orderedRender();
            light->endGenShadowMap();
            ELGLState::restoreState();
        }
    }
}