//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELNODE_H
#define EZGL_ELNODE_H

#include "types/EZGLTypes.h"
#include <vector>
#include <string>

/**
 * 所有的node的生命周期由parent管理，parent销毁时会销毁所有子node对象。主动销毁Node请使用release方法，禁止手动delete node;
**/

class ELNode {
public:
    ELNode * parent;
    // children nodes
    std::vector<ELNode *> children;
    ELTransform *transform;
    bool renderShadow;
    bool isTransparency;
    ELFloat elapsedSeconds;
    std::string identity;
    ELVector3 size;
    
    static bool drawTransparency;
public:
    ELNode();
    void release();

    void addNode(ELNode *node);
    // update node behavior
    virtual void update(ELFloat timeInSecs);
    // rerender node
    virtual void render();
    // 字符串描述的Node种类
    virtual std::string kind();

    // Node树的查找功能
    std::vector<ELNode *> findChildrenWithKind(std::string kind, bool deepSearch = false);
    ELNode * findChildWithIdentity(std::string identity, bool deepSearch = false);

    // Node Summary
    ELInt nodeSumCount();
    virtual std::string description();

    bool containTransparencyNode();
protected:
    bool objReleased;
    virtual ~ELNode();
    void findChildrenWithKind(std::string kind, std::vector<ELNode *> &collector, bool furtherSearch = true);
    void findChildWithIdentity(std::string kind, std::vector<ELNode *> &collector, bool furtherSearch = true);
};


#endif //EZGL_ELNODE_H
