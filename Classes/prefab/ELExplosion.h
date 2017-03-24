//
// Created by wangyang on 16/12/27.
//

#ifndef EZGL_ELEXPLOSION_H
#define EZGL_ELEXPLOSION_H

#include "../core/ELNode.h"

class ELWorld;

class ELExplosion : public ELNode {
public:
    ELExplosion(ELWorld *world, ELVector3 position);
};


#endif //EZGL_ELEXPLOSION_H
