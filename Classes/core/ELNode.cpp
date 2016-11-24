//
// Created by wangyang on 16/11/24.
//

#include "ELNode.h"

ELNode::~ELNode() {
    for (int i = 0; i < children.size(); ++i) {
        delete children.at(i);
    }
}

void ELNode::addNode(ELNode *node) {
    children.push_back(node);
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