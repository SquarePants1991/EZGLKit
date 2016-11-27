//
// Created by wangyang on 16/11/24.
//

#include <core/ELCamera.h>
#include "ELGeometry.h"
#include "ELEffect.h"
#include "core/ELGameObject.h"

ELGeometry::ELGeometry() : vao(-1) {
    material = ELMaterialDefault;
}

void ELGeometry::prepare() {
    data = generateData();
    setupVao();
    isGeometryDataValid = true;
}

void ELGeometry::setNeedRegenData() {
    isGeometryDataValid = false;
}

void ELGeometry::update(ELFloat timeInSecs) {

}

void ELGeometry::render() {
    if (isGeometryDataValid == false) {
        prepare();
    }

    ELEffect *defaultEffect = effect();
    ELMatrix4 matrix3 = ELMatrix4Identity;
//    glUseProgram(effect->program->value);
//
//    [self.world.effect applyToProgram:self.glProgram];

    defaultEffect->prepare();

    ELProgram *program = defaultEffect->program;

    ELGameObject *gameObj = (ELGameObject *)gameObject();
    ELCamera *camera = gameObj->mainCamera();

//    glUniform3fv([self.glProgram uniformWithStr:@"cameraPosition"], 1, ((EZGLPerspectiveCamera *)self.world.camera).transformedEye.v);
//
    glUniformMatrix4fv(program->uniform("viewProjection"), 1, 0, camera->matrix().m);
    glUniformMatrix4fv(program->uniform("modelMatrix"), 1, 0, ELMatrix4FromTransform(gameObj->transform).m);
//    glUniformMatrix3fv([self.glProgram uniform:UNIFORM_NORMAL_MATRIX], 1, 0, self.normalMatrix.m);
//    glUniformMatrix4fv([self.glProgram uniform:UNIFORM_LIGHT_VIEWPROJECTION], 1,0, self.lightViewProjection.m);
//
    glUniform4fv(program->uniform("material.ambient"), 1, material.ambient.v);
    glUniform4fv(program->uniform("material.diffuse"), 1, material.diffuse.v);
    glUniform4fv(program-> uniform("material.specular"), 1, material.specular.v);
//
//    glUniform1i([self.glProgram uniform:UNIFORM_DIFFUSE_MAP], 0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, self.material.diffuseMap);
//
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, self.material.shadowMap);
//    glUniform1i([self.glProgram uniform:UNIFORM_SHADOW_MAP], 1);
//
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, self.material.normalMap);
//    glUniform1i([self.glProgram uniformWithStr:@"normalMap"], 2);
//
//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_2D, self.material.specularMap);
//    glUniform1i([self.glProgram uniformWithStr:@"specularMap"], 3);
//
//    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_2D, self.material.ambientMap);
//    glUniform1i([self.glProgram uniformWithStr:@"ambientMap"], 4);

    glBindVertexArray(vao);
    if (data.supportIndiceVBO) {
        glDrawElements(GL_TRIANGLES, data.indiceCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
    }
    glBindVertexArray(0);
}

void ELGeometry::setupVao() {

    ELEffect * defaultEffect = effect();
    ELProgram *program = defaultEffect->program;
    if (vao < 0) {
        glGenVertexArrays(1, (GLuint *)(&vao));
    }
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);

    GLuint positionLocation = glGetAttribLocation(program->value, "position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, 0);

    GLuint normalLocation = glGetAttribLocation(program->value, "normal");
    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(3 * sizeof(GLfloat)));

    GLuint uvLocation = glGetAttribLocation(program->value, "texcoord");
    glEnableVertexAttribArray(uvLocation);
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(6 * sizeof(GLfloat)));

    GLuint tangentLocation = glGetAttribLocation(program->value, "tangent");
    glEnableVertexAttribArray(tangentLocation);
    glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(8 * sizeof(GLfloat)));

    GLuint bitangentLocation = glGetAttribLocation(program->value, "bitangent");
    glEnableVertexAttribArray(bitangentLocation);
    glVertexAttribPointer(bitangentLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(11 * sizeof(GLfloat)));

    if (data.indiceVBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indiceVBO);
    }

    glBindVertexArray(0);

}

ELEffect * ELGeometry::effect() {
    std::vector<ELNode *> effects = gameObject()->findChildrenWithKind("effect");
    if (effects.size() == 0) {
        return ELEffect::defaultEffect();
    }
    ELEffect *effect = dynamic_cast<ELEffect *>(effects.at(0));
    return effect;
}