//
// Created by wangyang on 16/11/25.
//

#include "ELLight.h"
#include "ELEffect.h"
#include "ELProgram.h"
#include "ELGameObject.h"
#include "ELWorld.h"
#include "ELProjector.h"

ELEffect::ELEffect() {
    kind = "effect";
}

ELEffect::ELEffect(const char *vertexShader,const char *fragmentShader) {
    program = std::shared_ptr<ELProgram>(new ELProgram(vertexShader, fragmentShader));
    kind = "effect";
}

ELEffect::~ELEffect() {
}

ELEffect * ELEffect::defaultEffect() {
    return new ELEffect();
}

void ELEffect::prepare() {
    glUseProgram(program->value);

    std::vector<ELNode *> lights = ((ELWorld *)parent)->findChildrenWithKind("light");
    std::vector<ELNode *> projectors = ((ELWorld *)parent)->findChildrenWithKind("projector");

    char buffer[128];

    for (int index = 0; index < lights.size(); ++index) {
        ELLight *light = (ELLight *)lights[index];
        snprintf(buffer, 1024, "lights[%d].type",index);
        glUniform1i(program->uniform(buffer), light->type);
        snprintf(buffer, 1024, "lights[%d].color",index);
        glUniform4fv(program->uniform(buffer), 1, (GLfloat *)light->color.v);
        snprintf(buffer, 1024, "lights[%d].position",index);
        glUniform3fv(program->uniform(buffer), 1, (GLfloat *)light->position.v);
        snprintf(buffer, 1024, "lights[%d].intensity",index);
        glUniform1f(program->uniform(buffer), light->intensity);
        snprintf(buffer, 1024, "lights[%d].intensityFallOff",index);
        glUniform1f(program->uniform(buffer), light->intensityFallOff);
    }
    glUniform1i(program->uniform("lightNum"), lights.size());

    for (int index = 0; index < projectors.size(); ++index) {
        ELProjector *projector = (ELProjector *)projectors[index];
        snprintf(buffer, 1024, "projectors[%d].viewProjection",index);
        glUniformMatrix4fv(program->uniform(buffer), 1, 0,(GLfloat *)projector->camera->matrix().m);
        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, projector->projectorMap);
        glUniform1i(program->uniform("projectorMap"), 11);
    }

    glUniform1i(program->uniform("projectorNum"), projectors.size());

    glUniform1f(program->uniform("frogStart"),frogStart);
    glUniform1f(program->uniform("frogEnd"),frogEnd);
    glUniform4fv(program->uniform("frogColor"), 1, (GLfloat *)frogColor.v);
    glUniform1f(program->uniform("time"), elapsedSeconds);
}

void ELEffect::active() {
    glUseProgram(program->value);
}
