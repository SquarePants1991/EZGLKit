//
// Created by wangyang on 16/11/24.
//

#include "../core/ELCamera.h"
#include "ELGeometry.h"
#include "../core/ELEffect.h"
#include "../core/ELGameObject.h"
#include "../utils/ELGLState.h"

bool ELGeometry::resetBorderBeforeUpdate = false;

ELGeometry::ELGeometry() : vao(-1),
                           isGeometryDataValid(false),
                           enableBorder(false),
                           borderWidth(0.1),
                           borderColor(ELVector4Make(1.0,1.0,1.0,1.0)),
                           onlyUseColorAttrib(false),
                           renderType(ELGeometryRenderTypeFrontSide)
{
    material = ELMaterialDefault;
    for (int i = 0;i < sizeof(materials) / sizeof(ELMaterial); ++i) {
        materials[i] = ELMaterialDefault;
    }
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
    if (ELGeometry::resetBorderBeforeUpdate) {
        enableBorder = false;
    }
    ELNode::update(timeInSecs);
}

std::string ELGeometry::kind() {
    return "geometry";
}

void ELGeometry::render() {
    if (!ELNode::drawTransparency && isTransparency) {
        return;
    }
    if (ELNode::drawTransparency && !isTransparency) {
        return;
    }

    if (isGeometryDataValid == false) {
        prepare();
    }


    ELEffect *defaultEffect = effect();
    defaultEffect->prepare();
    ELProgram *program = defaultEffect->program;
    ELCamera *camera = gameObject()->mainCamera();

    effectDidActive(defaultEffect);

    glUniform3fv(program->uniform("cameraPosition"), 1, (GLfloat *)camera->position().v);
    glUniformMatrix4fv(program->uniform("viewProjection"), 1, 0, (GLfloat *)camera->matrix().m);
    glUniformMatrix4fv(program->uniform("modelMatrix"), 1, 0, (GLfloat *)modelMatrix().m);
//    glUniform4fv(program->uniform("material.ambient"), 1, material.ambient.v);
//    glUniform4fv(program->uniform("material.diffuse"), 1, material.diffuse.v);
//    glUniform4fv(program->uniform("material.specular"), 1, material.specular.v);
//
//    glUniform1i(program->uniform("diffuseMap"), 0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, material.diffuseMap);

    int textureID = 0;
    for (int i = 0; i < sizeof(materials) / sizeof(ELMaterial); ++i) {
        ELMaterial mat = materials[i];
        char buffer[256];
        snprintf(buffer, 256, "materials[%d].ambient", i);
        glUniform4fv(program->uniform(buffer), 1, (GLfloat *)mat.ambient.v);
        snprintf(buffer, 256, "materials[%d].diffuse", i);
        glUniform4fv(program->uniform(buffer), 1, (GLfloat *)mat.diffuse.v);
        snprintf(buffer, 256, "materials[%d].specular", i);
        glUniform4fv(program->uniform(buffer), 1, (GLfloat *)mat.specular.v);
        snprintf(buffer, 256, "materials[%d].shininess", i);
        glUniform1f(program->uniform(buffer), (GLfloat)mat.shininess);

        snprintf(buffer, 256, "materials[%d].diffuseMap", i * 2 + 0);
        glUniform1i(program->uniform(buffer), textureID);
        GLenum gltexture = GL_TEXTURE0 + textureID;
        glActiveTexture(gltexture);
        glBindTexture(GL_TEXTURE_2D, mat.diffuseMap);
        textureID++;
        
        snprintf(buffer, 256, "materials[%d].specularMap", i * 2 + 0);
        glUniform1i(program->uniform(buffer), textureID);
        gltexture = GL_TEXTURE0 + textureID;
        glActiveTexture(gltexture);
        glBindTexture(GL_TEXTURE_2D, mat.specularMap);
        textureID++;
        
        if (mat.normalMap > 0) {
            snprintf(buffer, 256, "materials[%d].normalMap", i * 2 + 0);
            glUniform1i(program->uniform(buffer), textureID);
            GLenum gltexture = GL_TEXTURE0 + textureID;
            glActiveTexture(gltexture);
            glBindTexture(GL_TEXTURE_2D, mat.normalMap);
            textureID++;
            
            snprintf(buffer, 256, "materials[%d].normalMapEnable", i * 2 + 0);
            glUniform1i(program->uniform(buffer), 1);
        } else {
            snprintf(buffer, 256, "materials[%d].normalMapEnable", i * 2 + 0);
            glUniform1i(program->uniform(buffer), 0);
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    glUniform1i(program->uniform("onlyUseColorAttrib"), onlyUseColorAttrib? 1 : 0);

    glBindVertexArrayEL(vao);
    if (enableBorder) {
        // TODO: 增加管理Cull Face状态类，使Cull Face能够restore回去，暂时禁用边缘渲染
        ELGLState::saveState();
        ELGLState::set(GL_CULL_FACE_MODE, GL_FRONT);
        glUniform1i(program->uniform("renderBorder"), 1);
        glUniform1f(program->uniform("borderWidth"), borderWidth);
        glUniform4fv(program->uniform("borderColor"), 1, (GLfloat *)borderColor.v);
        if (data.supportIndiceVBO) {
            glDrawElements(GL_TRIANGLES, data.indiceCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
        }
        ELGLState::restoreState();
    }

//    ELGLState::saveState();
//    switch (renderType) {
//        case ELGeometryRenderTypeFrontSide:
//            ELGLState::set(ELGLStateCullFace, GL_BACK);
//            break;
//        case ELGeometryRenderTypeBackSide:
//            ELGLState::set(ELGLStateCullFace, GL_FRONT);
//            break;
//        case ELGeometryRenderTypeDoubleSide:
//            ELGLState::set(ELGLStateCullFace, GL_NONE);
//            break;
//    }

    glUniform1i(program->uniform("renderBorder"), 0);
    if (data.supportIndiceVBO) {
        glDrawElements(GL_TRIANGLES, data.indiceCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
    }
    glBindVertexArrayEL(0);

    ELNode::render();
//    ELGLState::restoreState();

    effectDidInactive(defaultEffect);
}


void ELGeometry::setupVao() {
    ELEffect * defaultEffect = effect();
    ELProgram *program = defaultEffect->program;
    if (vao < 0) {
        glGenVertexArraysEL(1, (GLuint *)(&vao));
    }
    glBindVertexArrayEL(vao);

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

    GLuint matIDLocation = glGetAttribLocation(program->value, "matID");
    glEnableVertexAttribArray(matIDLocation);
    glVertexAttribPointer(matIDLocation, 1, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(8 * sizeof(GLfloat)));

    GLuint tangentLocation = glGetAttribLocation(program->value, "tangent");
    glEnableVertexAttribArray(tangentLocation);
    glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(9 * sizeof(GLfloat)));

    GLuint bitangentLocation = glGetAttribLocation(program->value, "bitangent");
    glEnableVertexAttribArray(bitangentLocation);
    glVertexAttribPointer(bitangentLocation, 3, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(12 * sizeof(GLfloat)));

    if (data.supportColorAttrib) {
        GLuint colorLocation = glGetAttribLocation(program->value, "color");
        glEnableVertexAttribArray(colorLocation);
        glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, data.vertexStride, BUFFER_OFFSET(15 * sizeof(GLfloat)));
    }

    if (data.indiceVBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indiceVBO);
    }

    glBindVertexArrayEL(0);

}

ELMatrix4 ELGeometry::modelMatrix() {
    ELMatrix4 baseMatrix = ELMatrix4FromTransform(transform);
    ELMatrix4 parentMatrix = ELMatrix4FromTransform(gameObject()->transform);
    return ELMatrix4Multiply(parentMatrix, baseMatrix);
}

ELEffect * ELGeometry::effect() {
    return gameObject()->activeEffect();
}
