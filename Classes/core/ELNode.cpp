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

bool ELNode::drawTransparency = true;

ELNode::ELNode() : renderShadow(false), elapsedSeconds(0), objReleased(false) {
    transform = new ELTransform();
    transform->position = ELVector3Make(0.0, 0.0, 0.0);
    transform->quaternion = ELQuaternionMakeWithAngleAndAxis(0,0,1,0);
    transform->scale = ELVector3Make(1, 1, 1);
//    identity = "default";
}

void ELNode::release() {
    objReleased = true;
}

ELNode::~ELNode() {
    delete transform;
    for (int i = 0; i < children.size(); ++i) {
        delete children.at(i);
    }
    printf("Node Destroyed!!! \n");
}

void ELNode::addNode(ELNode *node) {
    children.push_back(node);
    node->parent = this;
}

void ELNode::update(ELFloat timeInSecs) {
    elapsedSeconds += timeInSecs;
    std::sort(children.begin(),children.end(),ELNodeTransparencyCompare());
    for (int i = 0; i < children.size(); ++i) {
        if (children.at(i)->objReleased) {
            delete children.at(i);
            children.erase(children.begin() + i);
            i--;
            continue;
        }
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
    } else {
        findChildrenWithKind(kind, components, false);
    }
    return components;
}

void ELNode::findChildrenWithKind(std::string kind, std::vector<ELNode *> &collector, bool furtherSearch) {
    for (int i = 0; i < children.size(); ++i) {
        if (furtherSearch) {
            children.at(i)->findChildrenWithKind(kind, collector);
        }
        ELNode *component = dynamic_cast<ELNode *>(children.at(i));
        if (component != NULL && component->kind() == kind) {
            collector.push_back(component);
        }
    }
}

ELNode * ELNode::findChildWithIdentity(std::string identity, bool deepSearch) {
    std::vector<ELNode *> components;
    if (deepSearch) {
        findChildWithIdentity(identity, components);
    } else {
        findChildWithIdentity(identity, components, false);
    }
    if (components.size() > 0) {
        return components.at(0);
    }
    return NULL;
}

void ELNode::findChildWithIdentity(std::string identity, std::vector<ELNode *> &collector, bool furtherSearch) {
    for (int i = 0; i < children.size(); ++i) {
        if (furtherSearch) {
            children.at(i)->findChildrenWithKind(identity, collector);
        }
        ELNode *component = dynamic_cast<ELNode *>(children.at(i));
        if (component != NULL && component->identity == identity) {
            collector.push_back(component);
        }
    }
}

ELInt ELNode::nodeSumCount() {
    ELInt sumCount = 1;
    for (int i = 0; i < children.size(); ++i) {
        sumCount += children.at(0)->nodeSumCount();
    }
    return sumCount;
}

std::string ELNode::description() {
    char buffer[2048];
    const char *fmt = "------------- ELNode ------------- \nIdentity(%s) Kind(%s) Children(%d) \nPosition(%f, %f, %f) \nQuaternion(%f, %f, %f, %f) \nScale(%f, %f, %f) \n";
    snprintf(buffer, 2048, fmt, identity.c_str(), kind().c_str(), children.size(),
             transform->position.x,
             transform->position.y,
             transform->position.z,
             transform->quaternion.x,
             transform->quaternion.y,
             transform->quaternion.z,
             transform->quaternion.w,
             transform->scale.x,
             transform->scale.y,
             transform->scale.z);
    return std::string(buffer);
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