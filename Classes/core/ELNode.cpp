//
// Created by wangyang on 16/11/24.
//

#include "ELNode.h"

class ELNodeTransparencyCompare {
public:
    bool operator()(ELNode *left,ELNode *right) {
        return left->containTransparencyNode();
    }
};

ELNode::ELNode() : renderShadow(false), elapsedSeconds(0) {
    transform = new ELTransform();
    transform->position = ELVector3Make(0.0, 0.0, 0.0);
    transform->quaternion = ELQuaternionMakeWithAngleAndAxis(0,0,1,0);
    transform->scale = ELVector3Make(1, 1, 1);
    identity = "default";
}

ELNode::~ELNode() {
    delete transform;
    for (int i = 0; i < children.size(); ++i) {
        delete children.at(i);
    }
}

void ELNode::addNode(ELNode *node) {
    children.push_back(node);
    node->parent = this;
}

void ELNode::update(ELFloat timeInSecs) {
    elapsedSeconds += timeInSecs;
    std::sort(children.begin(),children.end(),ELNodeTransparencyCompare());
    for (int i = 0; i < children.size(); ++i) {
        children.at(i)->update(timeInSecs);
    }
}

void ELNode::render() {
    for (int i = 0; i < children.size(); ++i) {
        children.at(i)->render();
    }
}

std::string ELNode::kind() {
    return std::string("Node");
}

std::vector<ELNode *> ELNode::findChildrenWithKind(std::string kind, bool deepSearch) {
    std::vector<ELNode *> components;
    if (deepSearch) {
        findChildrenWithKind(kind, components);
        return components;
    } else {
        return findChildrenWithKind(kind);
    }
}

void ELNode::findChildrenWithKind(std::string kind, std::vector<ELNode *> &collector) {
    for (int i = 0; i < children.size(); ++i) {
        children.at(i)->findChildrenWithKind(kind, collector);
        ELNode *component = dynamic_cast<ELNode *>(children.at(i));
        if (component != NULL && component->kind() == kind) {
            collector.push_back(component);
        }
    }
}

std::vector<ELNode *> ELNode::findChildrenWithKind(std::string kind) {
    std::vector<ELNode *> components;
    for (int i = 0; i < children.size(); ++i) {
        ELNode *component = dynamic_cast<ELNode *>(children.at(i));
        if (component != NULL && component->kind() == kind) {
            components.push_back(component);
        }
    }
    return components;
}

ELNode * ELNode::findChildWithIdentity(std::string identity) {
    std::vector<ELNode *> components;
    for (int i = 0; i < children.size(); ++i) {
        ELNode *component = dynamic_cast<ELNode *>(children.at(i));
        if (component != NULL && component->identity == identity) {
            return component;
        }
    }
    return NULL;
}

bool ELNode::containTransparencyNode() {
    if (isTransparency) {
        return true;
    }
    for (int i = 0; i < children.size(); ++i) {
        if (children.at(i)->containTransparencyNode()) {
            return true;
        }
    }
    return false;
}