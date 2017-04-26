//
// Created by wangyang on 16/12/27.
//

#ifndef EZGL_ELSNOW_H
#define EZGL_ELSNOW_H

#include "../core/ELNode.h"

class ELWorld;
class ELSnow : public ELNode {
public:
    ELSnow(std::shared_ptr<ELWorld> world, ELVector3 position, ELVector2 size = ELVector2Make(400, 400));
};


#endif //EZGL_ELSNOW_H
