//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELCOMPONENT_H
#define EZGL_ELCOMPONENT_H

#include "EZGLBase.h"
#include "ELNode.h"

class ELComponent : public ELNode{
public:
    ELComponent();

    ELNode *gameObject();
};


#endif //EZGL_ELCOMPONENT_H
