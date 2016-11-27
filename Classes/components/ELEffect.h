//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELEFFECT_H
#define EZGL_ELEFFECT_H

#include "EZGLBase.h"
#include "core/ELProgram.h"
#include "core/ELComponent.h"

class ELEffect : public ELComponent {
public:
    ELProgram *program;
    static ELEffect *defaultEffect();
public:
    ELEffect();
    ELEffect(const char *vertexShader,const char *fragmentShader);
    ~ELEffect();

    void prepare();

    virtual std::string kind();
};


#endif //EZGL_EZGLEFFECT_H
