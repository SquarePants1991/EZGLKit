//
// Created by wangyang on 16/12/19.
//

#include "ELGLState.h"

std::map<ELGLStates, GLenum> ELGLState::values = std::map<ELGLStates, GLenum>();
std::map<ELGLStates, GLenum> ELGLState::valuesSaved = std::map<ELGLStates, GLenum>();
std::map<ELGLStates, ELGLStateSetFunc> ELGLState::stateProcessFunc = std::map<ELGLStates, ELGLStateSetFunc>();

void ELGLState::setup() {
    ELGLState::stateProcessFunc[ELGLStateCullFace] = glCullFace;
}

void ELGLState::saveState() {
    ELGLState::valuesSaved = ELGLState::values;
}

void ELGLState::restoreState() {
    std::map<ELGLStates, GLenum>::iterator iter;
    for (iter = ELGLState::values.begin(); iter != ELGLState::values.end(); ++iter) {
        if (ELGLState::valuesSaved[iter->first] != iter->second) {
            set(iter->first, ELGLState::valuesSaved[iter->first]);
        }
    }
    ELGLState::values = ELGLState::valuesSaved;
}

void ELGLState::set(ELGLStates state, GLenum stateValue) {
    ELGLState::stateProcessFunc[ELGLStateCullFace](stateValue);
    ELGLState::values[state] = stateValue;
}


// convenience methods
void ELGLState::cullFace(GLenum cullFaceState) {
    set(ELGLStateCullFace, cullFaceState);
}