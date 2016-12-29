//
// Created by wangyang on 16/12/2.
//

#ifndef EZGL_FGSCENE_H
#define EZGL_FGSCENE_H

#include "EZGL.h"

class FGScene {
public:
    FGScene(ELWorld *world);

    void update(ELFloat timeInSecs);

    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
private:
    ELWorld *world;
};


#endif //EZGL_FGSCENE_H
