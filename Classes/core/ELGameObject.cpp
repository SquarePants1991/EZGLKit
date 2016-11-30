//
// Created by wangyang on 16/11/25.
//

#include "ELGameObject.h"
#include "ELComponent.h"
#include "ELLight.h"

ELGameObject::ELGameObject(ELWorld *world) : ELNode(), world(world) {
}

void ELGameObject::addComponent(ELComponent *component) {
    ELNode::addNode(dynamic_cast<ELNode *>(component));
    component->didAddedToGameObject(this);
}

void ELGameObject::render() {
    ELNode::render();
}

std::vector<ELLight *> ELGameObject::lights() {
    std::vector<ELLight *> lights = getNodesFromWorld<ELLight *>();
    return lights;
}

ELCamera * ELGameObject::mainCamera() {
    return world->mainCamera;
    std::vector<ELCamera *> cameras = getNodesFromWorld<ELCamera *>();
    if (cameras.size() > 0) {
        return cameras.at(0);
    }
    return new ELCamera();
}

template <typename T>
std::vector<T> ELGameObject::getNodesFromWorld() {
    std::vector<T> nodes;
    for (int i = 0; i < world->children.size(); ++i) {
        T node = dynamic_cast<T>(world->children.at(i));
        if (node != NULL) {
            nodes.push_back(node);
        }
    }
    return nodes;
}