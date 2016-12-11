//
// Created by wangyang on 16/12/9.
//

#include "utils/ELAssets.h"
#include "ELProjector.h"
#include "ELCamera.h"
#include "ELTexture.h"

ELProjector::ELProjector() {
    ELVector3 eye = {0, 1.7, 0};
    ELVector3 center = {0, 0, 0};
    ELVector3 up = {0, 0, 1};
    camera = new ELCamera(eye, center, up, 70.0, 1.0, 0.1, 1000);
    camera->identity = "p1";
    camera->asOrtho(-4, 4, 4, -4, -20,20);
    this->projectorMap = ELTexture::texture(ELAssets::shared()->findFile("1.jpg"))->value;
}

ELProjector::ELProjector(ELCamera *camera, ELUint projectorMap) {
    this->camera = camera;
    this->projectorMap = projectorMap;
}

std::string ELProjector::kind() {
    return "projector";
}

void ELProjector::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
    camera->rotateEye(timeInSecs,ELVector3Make(0,1,0));
}