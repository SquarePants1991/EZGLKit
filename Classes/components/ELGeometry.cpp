//
// Created by wangyang on 16/11/24.
//

#include <core/ELCamera.h>
#include "ELGeometry.h"
#include "core/ELEffect.h"
#include "core/ELGameObject.h"

ELGeometry::ELGeometry() : vao(-1),
                           isGeometryDataValid(false),
                           borderWidth(0.1),
                           borderColor(ELVector4Make(1.0,1.0,1.0,1.0))
{
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
    ELNode::update(timeInSecs);
    enableBorder = false;
}

std::string ELGeometry::kind() {
    return "geometry";
}

void ELGeometry::render() {
    if (isGeometryDataValid == false) {
        prepare();
    }

    ELEffect *defaultEffect = effect();
    ELProgram *program = defaultEffect->program;
    ELCamera *camera = gameObject()->mainCamera();

    glUniform3fv(program->uniform("cameraPosition"), 1, camera->position().v);

    glUniformMatrix4fv(program->uniform("viewProjection"), 1, 0, camera->matrix().m);
    glUniformMatrix4fv(program->uniform("modelMatrix"), 1, 0, ELMatrix4FromTransform(gameObject()->transform).m);
    glUniform4fv(program->uniform("material.ambient"), 1, material.ambient.v);
    glUniform4fv(program->uniform("material.diffuse"), 1, material.diffuse.v);
    glUniform4fv(program->uniform("material.specular"), 1, material.specular.v);

    glUniform1i(program->uniform("diffuseMap"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.diffuseMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, material.normalMap);
    glUniform1i(program->uniform("normalMap"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, material.specularMap);
    glUniform1i(program->uniform("specularMap"), 3);

    glBindVertexArray(vao);
    if (enableBorder) {
        glCullFace(GL_FRONT);
        glUniform1i(program->uniform("renderBorder"), 1);
        glUniform1f(program->uniform("borderWidth"), borderWidth);
        glUniform4fv(program->uniform("borderColor"), 1, borderColor.v);
        if (data.supportIndiceVBO) {
            glDrawElements(GL_TRIANGLES, data.indiceCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
        }
        glCullFace(GL_BACK);
    }

    glUniform1i(program->uniform("renderBorder"), 0);
    if (data.supportIndiceVBO) {
        glDrawElements(GL_TRIANGLES, data.indiceCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
    }
    glBindVertexArray(0);
}

void ELGeometry::setupVao() {
    printf("VAO Setup \n");
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
    return gameObject()->activeEffect();
}