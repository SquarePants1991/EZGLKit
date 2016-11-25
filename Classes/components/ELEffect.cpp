//
// Created by wangyang on 16/11/25.
//

#include "ELEffect.h"
#include "core/ELProgram.h"

ELEffect::ELEffect() {

}

ELEffect::ELEffect(const char *vertexShader,const char *fragmentShader) {
    program = new ELProgram(vertexShader, fragmentShader);
    GLuint loc = glGetAttribLocation(program->value, "position");
    loc = loc;
}

ELEffect::~ELEffect() {
    delete program;
}

ELEffect * ELEffect::defaultEffect() {
    return new ELEffect();
}

void ELEffect::prepare() {
    glUseProgram(program->value);
}

std::string ELEffect::kind() {
    return "effect";
}