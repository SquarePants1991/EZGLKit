//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELMESHGEOMETRY_H
#define EZGL_ELMESHGEOMETRY_H

#include "ELVertexBufferGeometry.h"
#include "../../utils/ELGeometryVertexBuffer.h"

class ELMeshGeometry : public ELVertexBufferGeometry {
public:
    std::map<std::string, ELAnimation> animations;
public:
    ELMeshGeometry();
    ELMeshGeometry(ELGeometryData data);

    void setAnimation(std::string animationName);

    virtual ELGeometryData generateData();
    void update(ELFloat timeInSecs);
    virtual void fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer);
    virtual void effectDidActive(ELEffect * effect);
private:
    ELGeometryData _data;
    std::vector<ELMatrix4> clustersMatrix;
    ELFloat currentTime;
    ELAnimation currentAnimation;
};


#endif //EZGL_ELMESHGEOMETRY_H
