//
// Created by wangyang on 16/12/9.
//

#ifndef EZGL_ELWATERPLANE_H
#define EZGL_ELWATERPLANE_H

#include "EZGLBase.h"
#include "ELNode.h"
#include "ELCamera.h"

class ELWaterPlane : public ELNode {
public:
    GLuint dudvMap;
    GLuint reflectionMap;
    ELWaterPlane();

    ELVector3 plane();
    virtual void render();
    virtual std::string kind();
};



#endif //EZGL_ELWATERPLANE_H
