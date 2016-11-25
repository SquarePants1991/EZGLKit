//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELPLANGEOMETRY_H
#define EZGL_ELPLANGEOMETRY_H

#include "../ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELPlanGeometry : public ELGeometry {
public:
    ELPlanGeometry(ELVector2 size);
    ~ELPlanGeometry();

    virtual void update(ELFloat timeInSecs);
    virtual void render();

    virtual ELGeometryData generateData();

private:
    ELVector2 size;
    ELGeometryVertexBuffer *vertexBuffer;
};


#endif //EZGL_ELPLANGEOMETRY_H
