//
// Created by wangyang on 16/11/25.
//

#include "ELGameObject.h"
#include "ELComponent.h"
#include "ELLight.h"

ELGameObject::ELGameObject(std::shared_ptr<ELWorld> world) : ELNode() {
    specificEffectName = "";
    this->world = world;
    kind = "game object";
}

void ELGameObject::addComponent(ELComponent *component) {
    ELNode::addNode(retain_ptr(ELComponent, component));
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
    return world.lock()->activedCamera;
}

ELEffect * ELGameObject::activeEffect() {
    if (specificEffectName != "") {
        ELEffect * effect = (ELEffect *)world.lock()->findChildWithIdentity(specificEffectName);
        if (effect != NULL) {
            return effect;
        }
    }
    return world.lock()->activedEffect;
}

template <typename T>
std::vector<T> ELGameObject::getNodesFromWorld() {
    std::vector<T> nodes;
    for (int i = 0; i < world.lock()->children.size(); ++i) {
        T node = dynamic_cast<T>(world.lock()->children.at(i).get());
        if (node != NULL) {
            nodes.push_back(node);
        }
    }
    return nodes;
}
