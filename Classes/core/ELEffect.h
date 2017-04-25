//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELEFFECT_H
#define EZGL_ELEFFECT_H

#include "../EZGLBase.h"
#include "ELProgram.h"
#include "ELNode.h"

class ELEffect : public ELNode {
public:
    std::shared_ptr<ELProgram> program;
    static ELEffect *defaultEffect();

    ELFloat frogStart;
    ELFloat frogEnd;
    ELVector4 frogColor;
public:
    ELEffect();
    ELEffect(const char *vertexShader,const char *fragmentShader);
    ~ELEffect();

    void prepare();
    void active();
};


#endif //EZGL_EZGLEFFECT_H
