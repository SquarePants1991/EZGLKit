//
//  EL2DPlane.hpp
//  EZGLSDK
//
//  Created by wangyang on 2017/3/17.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#ifndef EL2DPlane_hpp
#define EL2DPlane_hpp


#include "../ELGeometry.h"
#include "../../utils/ELGeometryVertexBuffer.h"

class EL2DPlane : public ELGeometry {
public:
    EL2DPlane(ELVector2 size);
    ~EL2DPlane();
    
    virtual ELGeometryData generateData();
    virtual void render();
    virtual void update(ELFloat timeInSecs);
    
private:
    ELVector2 size;
    ELGeometryVertexBuffer *vertexBuffer;
};


#endif /* EL2DPlane_hpp */
