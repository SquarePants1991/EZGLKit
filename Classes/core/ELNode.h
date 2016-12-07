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
    bool renderShadow;
    bool isTransparency;

public:
    ELNode();

    ELNode * parent;
    // children nodes
    std::vector<ELNode *> children;
    void addNode(ELNode *node);

    std::string identity;

    // update node behavior
    virtual void update(ELFloat timeInSecs);
    // rerender node
    virtual void render();

    virtual std::string kind();

    std::vector<ELNode *> findChildrenWithKind(std::string kind);
    ELNode * findChildWithIdentity(std::string identity);
    bool containTransparencyNode();
protected:
    ~ELNode();
};


#endif //EZGL_ELNODE_H
