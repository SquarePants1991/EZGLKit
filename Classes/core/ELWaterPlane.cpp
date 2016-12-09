//
// Created by wangyang on 16/12/9.
//

#include "ELWaterPlane.h"

ELWaterPlane::ELWaterPlane() {

}

ELVector3 ELWaterPlane::plane() {
    return ELVector3Make(0,1,0);
}

void ELWaterPlane::render() {
    ELNode::render();
}

std::string ELWaterPlane::kind() {
    return "water_plane";
}