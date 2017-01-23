//
// Created by wangyang on 16/12/28.
//

#ifndef EZGL_ELRENDERPASS_H
#define EZGL_ELRENDERPASS_H

class ELWorld;
class ELRenderPass {
public:
    virtual void render(ELWorld *world);
};


#endif //EZGL_ELRENDERPASS_H
