//
// Created by wangyang on 16/12/9.
//

#include "../utils/ELAssets.h"
#include "ELProjector.h"
#include "ELCamera.h"
#include "ELTexture.h"

ELProjector::ELProjector() {
    ELVector3 eye = {30, 3, 30};
    ELVector3 center = {30, 0, 30};
    ELVector3 up = {0, 0, 1};
    camera = retain_ptr_init_v(ELCamera, eye, center, up, 70.0, 1.0, 0.1, 1000);
    camera->identity = "p1";
    camera->ortho(-15, 15, 15, -15, -20, 20);
    // TODO: 该代码和析构代码冲突，考虑传递ELTexture进来而不是TextureID
    this->projectorMap = ELTexture::texture(ELAssets::shared()->findFile("icon.png"))->value;
    this->kind = "projector";
}

ELProjector::ELProjector(ELCamera *camera, ELUint projectorMap) {
    this->camera = retain_ptr(ELCamera, camera);
    this->projectorMap = projectorMap;
}

ELProjector::~ELProjector() {
    // TODO: 该代码和默认初始化代码冲突，考虑传递ELTexture进来而不是TextureID，暂时注释。
    // glDeleteTextures(1, &projectorMap);
}

void ELProjector::update(ELFloat timeInSecs) {
    ELNode::update(timeInSecs);
    camera->rotateEye(timeInSecs,ELVector3Make(0,1,0));
}