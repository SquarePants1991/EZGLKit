//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELMESHGEOMETRY_H
#define EZGL_ELMESHGEOMETRY_H

#include "ELVertexBufferGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELMeshGeometry : public ELVertexBufferGeometry {
public:
    ELMeshGeometry();
    ELMeshGeometry(ELGeometryData data);

    virtual ELGeometryData generateData();
    void update(ELFloat timeInSecs);
    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer);
private:
    ELGeometryData _data;
};


#endif //EZGL_ELMESHGEOMETRY_H
