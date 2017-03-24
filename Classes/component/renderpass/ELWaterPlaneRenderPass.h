//
// Created by wangyang on 16/12/28.
//

#ifndef EZGL_ELSIMPLERENDERPASS_H
#define EZGL_ELSIMPLERENDERPASS_H

#include "../../core/ELRenderPass.h"

class ELWaterPlaneRenderPass : public ELRenderPass {
public:
    virtual void render(ELWorld *world);

private:
    void renderReflectionMaps(ELWorld *world);
    void renderRefractionMaps(ELWorld *world);
};


#endif //EZGL_ELSIMPLERENDERPASS_H
