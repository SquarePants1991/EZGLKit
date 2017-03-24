//
// Created by wangyang on 16/12/28.
//

#include "ELWaterPlaneRenderPass.h"
#include "../../component/ELWaterPlane.h"
#include "../../utils/ELGLState.h"
#include "../../core/ELWorld.h"
#include "../../core/ELGameObject.h"

void ELWaterPlaneRenderPass::render(ELWorld *world) {
    renderReflectionMaps(world);
    renderRefractionMaps(world);
}

void ELWaterPlaneRenderPass::renderReflectionMaps(ELWorld *world) {
    world->activeEffect("render_scene");
    world->activedEffect->prepare();
    world->activeCamera("main");
    glEnable(GL_DEPTH_TEST);
    std::vector<ELNode *> waterPlanes = world->findChildrenWithKind("water_plane", true);
    ELWaterPlane::isInWaterPlanePreparePass = true;
    for (int i = 0; i < waterPlanes.size(); ++i) {
        ELWaterPlane * waterPlane = dynamic_cast<ELWaterPlane *>(waterPlanes.at(i));
        if (waterPlane != NULL) {
            ELVector4 waterPlaneVector = waterPlane->plane();
            if (world->activedCamera->position().y < waterPlaneVector.y) {
                //under water
                waterPlaneVector.y = -waterPlaneVector.y;
            }

            glUniform1i(world->activedEffect->program->uniform("useAdditionMatrix"),1);
            ELMatrix4 additionMatrix = ELMatrix4MakeScale(1,-1,1);
            additionMatrix = ELMatrix4Multiply(additionMatrix,ELMatrix4MakeTranslation(0,waterPlane->gameObject()->transform->position.y,0));
            glUniformMatrix4fv(world->activedEffect->program->uniform("additionMatrix"), 1, 0, (GLfloat *)additionMatrix.m);
            ELGLState::set(GL_CULL_FACE_MODE, GL_FRONT);
            glUniform1i(world->activedEffect->program->uniform("enableClipPlane0"), 1);
            glUniform4fv(world->activedEffect->program->uniform("clipPlane0"), 1,(GLfloat *)waterPlaneVector.v);
            waterPlane->beginGenReflectionMap();
            world->orderedRender();
            waterPlane->endGenReflectionMap();
            glUniform1i(world->activedEffect->program->uniform("enableClipPlane0"), 0);
        }
    }
    ELWaterPlane::isInWaterPlanePreparePass = false;
    ELGLState::set(GL_CULL_FACE_MODE, GL_BACK);
    glUniform1i(world->activedEffect->program->uniform("useAdditionMatrix"),0);
    glUniformMatrix4fv(world->activedEffect->program->uniform("additionMatrix"), 1, 0, (GLfloat *)ELMatrix4Identity.m);
}

void ELWaterPlaneRenderPass::renderRefractionMaps(ELWorld *world) {
    world->activeEffect("render_scene");
    world->activedEffect->prepare();
    world->activeCamera("main");
    std::vector<ELNode *> waterPlanes = world->findChildrenWithKind("water_plane", true);
    ELWaterPlane::isInWaterPlanePreparePass = true;
    for (int i = 0; i < waterPlanes.size(); ++i) {
        ELWaterPlane * waterPlane = dynamic_cast<ELWaterPlane *>(waterPlanes.at(i));
        if (waterPlane != NULL) {
            ELVector4 waterPlaneVector = waterPlane->inversePlane();
            if (world->activedCamera->position().y < waterPlaneVector.y) {
                //under water
                waterPlaneVector.y = -waterPlaneVector.y;
            }

            glUniform1i(world->activedEffect->program->uniform("enableClipPlane0"), 1);
            glUniform4fv(world->activedEffect->program->uniform("clipPlane0"), 1,(GLfloat *)waterPlaneVector.v);
            glUniform1i(world->activedEffect->program->uniform("useAdditionMatrix"),0);
            waterPlane->beginGenRefractionMap();
            world->orderedRender();
            waterPlane->endGenRefractionMap();
            glUniform1i(world->activedEffect->program->uniform("enableClipPlane0"), 0);
        }
    }
    ELWaterPlane::isInWaterPlanePreparePass = false;
}
