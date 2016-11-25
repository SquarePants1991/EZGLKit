//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELLIGHT_H
#define EZGL_ELLIGHT_H

#include "EZGLBase.h"
#include "ELNode.h"

class ELLight : public ELNode {
public:
    ELVector4 color;
    ELFloat intensity;
    ELVector4 position;

public:
    virtual std::string kind();
    virtual std::string identity();
};


#endif //EZGL_ELLIGHT_H
