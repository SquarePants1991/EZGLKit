//
// Created by wangyang on 16/12/19.
//

#ifndef EZGL_ELGLSTATE_H
#define EZGL_ELGLSTATE_H

#include "EZGLBase.h"

enum _ELGLStates {
    ELGLStateCullFace
};
typedef _ELGLStates ELGLStates;

typedef void(* ELGLStateSetFunc)(GLenum stateValue);

class ELGLState {
public:
    static void setup();
    static void saveState();
    static void restoreState();

    static void cullFace(GLenum cullFaceState);
    static void set(ELGLStates state, GLenum stateValue);
private:
    static std::map<ELGLStates, GLenum> values;
    static std::map<ELGLStates, GLenum> valuesSaved;
    static std::map<ELGLStates, ELGLStateSetFunc> stateProcessFunc;
};


#endif //EZGL_ELGLSTATE_H
