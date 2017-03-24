//
// Created by wangyang on 16/12/19.
//

#ifndef EZGL_ELGLSTATE_H
#define EZGL_ELGLSTATE_H

#include "../EZGLBase.h"

typedef void(* ELGLStateSetFunc)(GLenum stateValue);

class ELGLState {
public:
    static void setup();
    static void saveState();
    static void restoreState();

    static void cullFace(GLenum cullFaceState);
    static void set(GLenum state, GLenum stateValue);
private:
    static std::map<GLenum, GLenum> values;
    static std::vector<std::map<GLenum, GLenum> > valuesSaved;
    static std::map<GLenum, ELGLStateSetFunc> stateProcessFunc;
    static std::map<GLenum, std::string> stateProcessFuncDesc;
};


#endif //EZGL_ELGLSTATE_H
