//
// Created by wangyang on 16/12/19.
//

#include "ELGLState.h"

std::map<GLenum, GLenum> ELGLState::values = std::map<GLenum, GLenum>();
std::vector<std::map<GLenum, GLenum> > ELGLState::valuesSaved = std::vector<std::map<GLenum, GLenum> >();
std::map<GLenum, ELGLStateSetFunc> ELGLState::stateProcessFunc = std::map<GLenum, ELGLStateSetFunc>();
std::map<GLenum, std::string> ELGLState::stateProcessFuncDesc = std::map<GLenum, std::string>();

void fake_glEnableOrDisable(GLenum state) {}
void glEnableOrDisable(GLenum state, GLenum stateVal) {
    if (stateVal == GL_TRUE) {
        glEnable(state);
    } else {
        glDisable(state);
    }
}


void ELGLState::setup() {
#define RegisterStateFunc(State, Func) \
    ELGLState::stateProcessFunc[State] = Func; \
    ELGLState::stateProcessFuncDesc[State] = "State-Func";
#define RegisterEnableFunc(State) \
    ELGLState::stateProcessFunc[State] = fake_glEnableOrDisable; \
    ELGLState::stateProcessFuncDesc[State] = "State";

    // enable or disable funcs
    RegisterEnableFunc(GL_DEPTH_TEST);
    RegisterEnableFunc(GL_CULL_FACE);

    // set func
    RegisterStateFunc(GL_CULL_FACE_MODE, glCullFace);
}

void ELGLState::saveState() {
    ELGLState::valuesSaved.push_back(ELGLState::values);
}

void ELGLState::restoreState() {
    if (ELGLState::valuesSaved.size() <= 0) {
        printf("ELGLState restore failed,nothing to restore.");
        return;
    }
    std::map<GLenum, GLenum> lastSaveValues = ELGLState::valuesSaved.at(0);
    std::map<GLenum, GLenum>::iterator iter;
    for (iter = ELGLState::values.begin(); iter != ELGLState::values.end(); ++iter) {
        if (lastSaveValues[iter->first] != iter->second) {
            set(iter->first, lastSaveValues[iter->first]);
        }
    }
    ELGLState::values = ELGLState::valuesSaved.at(0);
    ELGLState::valuesSaved.erase(ELGLState::valuesSaved.begin());
}

void ELGLState::set(GLenum state, GLenum stateValue) {
    ELGLStateSetFunc setFunc = ELGLState::stateProcessFunc[state];
    if (setFunc == fake_glEnableOrDisable) {
        glEnableOrDisable(state, stateValue);
    } else {
        setFunc(stateValue);
    }
    ELGLState::values[state] = stateValue;
}


// convenience methods
void ELGLState::cullFace(GLenum cullFaceState) {
    set(GL_CULL_FACE_MODE, cullFaceState);
}
