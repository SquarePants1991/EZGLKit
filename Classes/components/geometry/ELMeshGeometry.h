//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELMESHGEOMETRY_H
#define EZGL_ELMESHGEOMETRY_H

#include "../ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELMeshGeometry : public ELGeometry {
public:
    ELMeshGeometry(ELGeometryData data);

    virtual ELGeometryData generateData();

private:
    ELGeometryData _data;
};


#endif //EZGL_ELMESHGEOMETRY_H
