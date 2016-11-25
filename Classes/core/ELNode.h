//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELNODE_H
#define EZGL_ELNODE_H

#include "types/EZGLTypes.h"
#include <vector>
#include <string>

class ELNode {
public:

    ELTransform *transform;
    ELNode();

    ELNode * parent;
    // children nodes
    std::vector<ELNode *> children;
    void addNode(ELNode *node);

    // update node behavior
    virtual void update(ELFloat timeInSecs);
    // rerender node
    virtual void render();

    virtual std::string kind();
    virtual std::string identity();

    std::vector<ELNode *> findChildrenWithKind(std::string kind);
    ELNode * findChildWithIdentity(std::string identity);

protected:
    ~ELNode();
};


#endif //EZGL_ELNODE_H
