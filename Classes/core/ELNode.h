//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELNODE_H
#define EZGL_ELNODE_H

#include "types/EZGLTypes.h"
#include <vector>

class ELNode {
public:
    // children nodes
    std::vector<ELNode *> children;
    void addNode(ELNode *node);

    // update node behavior
    virtual void update(ELFloat timeInSecs);
    // rerender node
    virtual void render();

protected:
    ~ELNode();
};


#endif //EZGL_ELNODE_H
