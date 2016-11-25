//
// Created by wangyang on 16/11/24.
//

#include "ELNode.h"

ELNode::ELNode() {
    transform = new ELTransform();
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

std::string ELNode::identity() {
    return std::string("default");
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
        if (component != NULL && component->identity() == identity) {
            return component;
        }
    }
    return NULL;
}