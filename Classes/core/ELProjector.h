//
// Created by wangyang on 16/12/9.
//

#ifndef EZGL_ELPROJECTOR_H
#define EZGL_ELPROJECTOR_H

#include "ELNode.h"

class ELCamera;

class ELProjector : public ELNode {
public:
    prop_strong(ELCamera, camera);
    ELUint projectorMap;
public:
    ELProjector();
    ELProjector(ELCamera *camera,ELUint projectorMap);
    ~ELProjector();
    virtual std::string kind();
    virtual void update(ELFloat timeInSecs);
};


#endif //EZGL_ELPROJECTOR_H
