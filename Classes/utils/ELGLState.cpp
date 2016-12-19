//
// Created by wangyang on 16/12/19.
//

#include "ELGLState.h"

std::map<ELGLStates, GLenum> ELGLState::values;
std::map<ELGLStates, GLenum> ELGLState::valuesSaved;
std::map<ELGLStates, ELGLStateSetFunc> ELGLState::stateProcessFunc;

void ELGLState::setup() {
    stateProcessFunc[ELGLStateCullFace] = glCullFace;
}

void ELGLState::saveState() {
    valuesSaved = values;
}

void ELGLState::restoreState() {
    values = valuesSaved;
    std::map<ELGLStates, GLenum>::iterator iter;
    for (iter = values.begin(); iter != values.end(); ++iter) {
        if (valuesSaved[iter->first] != iter->first) {
            set(iter->first, valuesSaved[iter->first]);
        }
    }
}

void ELGLState::set(ELGLStates state, GLenum stateValue) {
    stateProcessFunc[ELGLStateCullFace](stateValue);
}


// convenience methods
void ELGLState::cullFace(GLenum cullFaceState) {
    set(ELGLStateCullFace, cullFaceState);
}