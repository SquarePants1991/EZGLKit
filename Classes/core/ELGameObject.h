//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELGAMEOBJECT_H
#define EZGL_ELGAMEOBJECT_H

#include "ELNode.h"
#include <vector>
#include <string>
#include "ELWorld.h"
#include "ELComponent.h"

class ELComponent;
class ELLight;
class ELCamera;
class ELEffect;
class ELGameObject : public ELNode {
public:
    ELGameObject(std::shared_ptr<ELWorld> world);
    void addComponent(ELComponent *component);
    virtual void render();

    template <typename T>
    std::vector<T> getNodesFromWorld();

    // for component
    std::vector<ELLight *> lights();
    ELCamera *mainCamera();
    ELEffect *activeEffect();

    std::string specificEffectName;
//private:
    std::weak_ptr<ELWorld> world;
};


#endif //EZGL_ELGAMEOBJECT_H
