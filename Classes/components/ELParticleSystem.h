//
// Created by wangyang on 16/12/6.
//

#ifndef EZGL_ELPARTICALSYSTEM_H
#define EZGL_ELPARTICALSYSTEM_H

#include "EZGLBase.h"
#include "ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"
#include "ELPartical.h"

class ELPartical;

struct _ELParticleSystemData {
    ELFloat maxParticleAmount;
    ELFloat birthRate;
    ELVector3 force;

    ELVector3 velocity;
    ELVector3 velocityRandomRangeBegin;
    ELVector3 velocityRandomRangeEnd;

    ELVector3 positionRandomRangeBegin;
    ELVector3 positionRandomRangeEnd;

    ELFloat sizeBegin;
    ELFloat sizeBeginRandomRangeBegin;
    ELFloat sizeBeginRandomRangeEnd;

    ELFloat sizeEnd;
    ELFloat sizeEndRandomRangeBegin;
    ELFloat sizeEndRandomRangeEnd;

    ELVector4 colorBegin;
    ELVector4 colorBeginRandomRangeBegin;
    ELVector4 colorBeginRandomRangeEnd;

    ELVector4 colorEnd;
    ELVector4 colorEndRandomRangeBegin;
    ELVector4 colorEndRandomRangeEnd;

    ELFloat age;
    ELFloat ageRandomBegin;
    ELFloat ageRandomEnd;
};
typedef _ELParticleSystemData ELParticleSystemData;

class ELParticleSystem : public ELGeometry {
public:
    ELParticleSystemData data;
public:
    ELParticleSystem();

    void restartPartical(ELPartical *partical);
    void setData(ELParticleSystemData data);

    virtual ELGeometryData generateData();
    virtual void update(ELFloat timeInSecs);
    virtual void render();

private:
    ELGeometryVertexBuffer *vertexBuffer;
    ELGeometryData currentData;
    bool isDataInitialized;
    ELPartical *partical;
    std::vector<ELPartical *> inactiveParticals;
    std::vector<ELPartical *> activeParticals;
    ELFloat emitTimeInterval;

    bool sortByDistance(ELPartical *obj1,ELPartical *obj2);
};


#endif //EZGL_ELPARTICALSYSTEM_H
