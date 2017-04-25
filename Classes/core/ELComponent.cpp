//
// Created by wangyang on 16/11/25.
//

#include "ELComponent.h"
#include "ELGameObject.h"

ELComponent::ELComponent() {
    kind = "component";
}

ELGameObject *ELComponent::gameObject() {
    return dynamic_cast<ELGameObject *>(parent);
}



