//
// Created by wangyang on 16/12/28.
//

#include "ELShadowMapRenderPass.h"
#include "core/ELWorld.h"
#include "core/ELLight.h"

void ELShadowMapRenderPass::render(ELWorld *world) {
    for (int i = 0; i < world->children.size(); ++i) {
        ELLight * light = dynamic_cast<ELLight *>(world->children.at(i));
        if (light != NULL && light->isShadowEnabled) {
            world->activeEffect("gen_shadow");
            world->activedEffect->prepare();
            light->beginGenShadowMap();
            world->activeCamera(light->identity + "-shadow-camera", light->shadowMapGenCamera());
            world->orderedRender();
            light->endGenShadowMap();
        }
    }
}