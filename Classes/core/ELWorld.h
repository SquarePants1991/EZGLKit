//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELWORLD_H
#define EZGL_ELWORLD_H

#include "ELNode.h"
#include <vector>

class ELWorld : public ELNode {
private:

public:
    virtual void update(ELFloat timeInSecs);
    virtual void render();
};


#endif //EZGL_ELWORLD_H
