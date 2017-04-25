//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELNODE_H
#define EZGL_ELNODE_H

#include "../EZGLBase.h"
#include "../types/EZGLTypes.h"
#include <vector>
#include <string>

/**
 * 所有的node的生命周期由parent管理，parent销毁时会销毁所有子node对象。主动销毁Node请使用release方法，禁止手动delete node;
**/

class ELNode {
public:
    ELNode* parent;
    // children nodes
    std::vector<std::shared_ptr<ELNode> > children;
    std::shared_ptr<ELTransform> transform;
    bool renderShadow;
    bool isTransparency;
    ELFloat elapsedSeconds;
    std::string identity;
    std::string kind;
    ELVector3 size;
    
    static bool drawTransparency;
public:
    ELNode();
    virtual ~ELNode();
    void release();

    void addNode(std::shared_ptr<ELNode> node);
    // update node behavior
    virtual void update(ELFloat timeInSecs);
    // rerender node
    virtual void render();

    // Node树的查找功能
    std::vector<ELNode *> findChildrenWithKind(std::string kind, bool deepSearch = false);
    ELNode * findChildWithIdentity(std::string identity, bool deepSearch = false);

    // Node Summary
    ELInt nodeSumCount();
    virtual std::string description();

    bool containTransparencyNode();
protected:
    bool objReleased;
    void findChildrenWithKind(std::string kind, std::vector<ELNode *> &collector, bool furtherSearch = true);
    void findChildWithIdentity(std::string kind, std::vector<ELNode *> &collector, bool furtherSearch = true);
};


#endif //EZGL_ELNODE_H
