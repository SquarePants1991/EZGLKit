//
// Created by wangyang on 16/12/28.
//

#ifndef EZGL_ELSHADOWMAPRENDERPASS_H
#define EZGL_ELSHADOWMAPRENDERPASS_H

#include "../../core/ELRenderPass.h"

class ELShadowMapRenderPass : public ELRenderPass {
public:
    virtual void render(ELWorld *world);
};


#endif //EZGL_ELSHADOWMAPRENDERPASS_H
