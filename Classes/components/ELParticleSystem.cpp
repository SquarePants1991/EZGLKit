//
// Created by wangyang on 16/12/6.
//

#include <types/ELGeometryTypes.h>
#include "ELParticleSystem.h"
#include "ELPartical.h"
#include "core/ELGameObject.h"
#include "ELGeometry.h"

#include <functional>

class ELParticalSystemCompare {
public:
    ELParticleSystem * system;
    ELParticalSystemCompare(ELParticleSystem *system) {
        this->system = system;
    }
    bool operator()(ELPartical *obj1,ELPartical *obj2)
    {
        ELFloat distance1 = ELVector3Distance(obj1->transform.position,this->system->gameObject()->mainCamera()->position());
        ELFloat distance2 = ELVector3Distance(obj2->transform.position,this->system->gameObject()->mainCamera()->position());
        return distance1 > distance2;
    }
};

ELParticleSystem::ELParticleSystem() : isDataInitialized(false), partical(NULL) {
    emitTimeInterval = 0.3;
    onlyUseColorAttrib = true;
}

ELGeometryData ELParticleSystem::generateData() {
    if (isDataInitialized == false) {
        vertexBuffer = new ELGeometryVertexBuffer();
        vertexBuffer->enableColorAttrib();
    } else {
        vertexBuffer->clear();
    }

    ELGeometryData data;

    for (int i = 0; i < activeParticals.size(); ++i) {
        ELPartical *partical = activeParticals.at(i);
        vertexBuffer->append(partical->quadRect);
    }


    GLfloat *vertex = (GLfloat *)(vertexBuffer->data());
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer->rawLength(), vertex, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryColorVertex);
    data.vertexStride = sizeof(ELGeometryColorVertex);
    data.supportIndiceVBO = false;
    data.supportColorAttrib = true;
    currentData = data;
    isDataInitialized = true;
    return data;
}

void ELParticleSystem::update(ELFloat timeInSecs) {

    // emit new particals
    emitTimeInterval -= timeInSecs;
    if (emitTimeInterval <= 0) {
        emitTimeInterval = 0.2;
        int birthAmount = data.birthRate * data.maxParticleAmount;
        for (int i = 0; i < birthAmount; ++i) {
            if (inactiveParticals.size() > 0) {
                restartPartical(inactiveParticals.at(0));
                activeParticals.push_back(inactiveParticals.at(0));
                inactiveParticals.erase(inactiveParticals.begin());
            }
        }
    }

    // update active particals
    for (int i = 0; i < activeParticals.size(); ++i) {
        ELPartical *partical = activeParticals.at(i);
        partical->update(gameObject()->mainCamera()->position(), timeInSecs, data.force);
        if (partical->isAlive() == false) {
            inactiveParticals.push_back(activeParticals.at(i));
            activeParticals.erase(activeParticals.begin() + i);
            i--;
        }
    }
    ELGeometry::update(timeInSecs);
}

void ELParticleSystem::setData(ELParticleSystemData data) {
    this->data = data;
    for (int i = 0; i < data.maxParticleAmount; ++i) {
        ELPartical *partical = new ELPartical();
        inactiveParticals.push_back(partical);
    }
}

ELVector2 randomVec2(ELVector2 initial,ELVector2 rangeBegin,ELVector2 rangeEnd) {
    srand(rand());
    ELFloat x = initial.x + rangeBegin.x + (rangeEnd.x - rangeBegin.x) * (rand() / (ELFloat)RAND_MAX);
    ELFloat y = initial.y + rangeBegin.y + (rangeEnd.y - rangeBegin.y) * (rand() / (ELFloat)RAND_MAX);
    return ELVector2Make(x,y);
}

ELVector2 randomVec2Lock(ELVector2 initial,ELFloat rangeBegin,ELFloat rangeEnd) {
    srand(rand());
    ELFloat randomVal = (rangeEnd - rangeBegin) * (rand() / (ELFloat)RAND_MAX);
    ELFloat x = initial.x + rangeBegin + randomVal;
    ELFloat y = initial.y + rangeBegin + randomVal;
    return ELVector2Make(x,y);
}

ELVector3 randomVec3(ELVector3 initial,ELVector3 rangeBegin,ELVector3 rangeEnd) {
    srand(rand());
    ELFloat x = initial.x + rangeBegin.x + (rangeEnd.x - rangeBegin.x) * (rand() / (ELFloat)RAND_MAX);
    ELFloat y = initial.y + rangeBegin.y + (rangeEnd.y - rangeBegin.y) * (rand() / (ELFloat)RAND_MAX);
    ELFloat z = initial.z + rangeBegin.z + (rangeEnd.z - rangeBegin.z) * (rand() / (ELFloat)RAND_MAX);
    return ELVector3Make(x,y,z);
}

ELVector4 randomVec4(ELVector4 initial,ELVector4 rangeBegin,ELVector4 rangeEnd) {
    srand(rand());
    ELFloat x = initial.x + rangeBegin.x + (rangeEnd.x - rangeBegin.x) * (rand() / (ELFloat)RAND_MAX);
    ELFloat y = initial.y + rangeBegin.y + (rangeEnd.y - rangeBegin.y) * (rand() / (ELFloat)RAND_MAX);
    ELFloat z = initial.z + rangeBegin.z + (rangeEnd.z - rangeBegin.z) * (rand() / (ELFloat)RAND_MAX);
    ELFloat w = initial.w + rangeBegin.w + (rangeEnd.w - rangeBegin.w) * (rand() / (ELFloat)RAND_MAX);
    return ELVector4Make(x,y,z,w);
}

void ELParticleSystem::restartPartical(ELPartical *partical) {
    partical->size = ELVector2Make(0.3,0.3);
    partical->transform = *(gameObject()->transform);
    srand(rand());

    partical->transform.position = randomVec3(partical->transform.position,data.positionRandomRangeBegin,data.positionRandomRangeEnd);
    partical->velocity = randomVec3(data.velocity,data.velocityRandomRangeBegin,data.velocityRandomRangeEnd);
    partical->sizeStart = randomVec2Lock(ELVector2Make(data.sizeBegin,data.sizeBegin),data.sizeBeginRandomRangeBegin,data.sizeBeginRandomRangeEnd);
    partical->sizeEnd = randomVec2Lock(ELVector2Make(data.sizeEnd,data.sizeEnd),data.sizeEndRandomRangeBegin,data.sizeEndRandomRangeEnd);
    partical->colorStart = randomVec4(data.colorBegin,data.colorBeginRandomRangeBegin,data.colorBeginRandomRangeEnd);
    partical->colorEnd = randomVec4(data.colorEnd,data.colorEndRandomRangeBegin,data.colorEndRandomRangeEnd);
    partical->age = data.age + data.ageRandomBegin + ( data.ageRandomEnd -  data.ageRandomBegin) * (rand() / (ELFloat)RAND_MAX);
    partical->reset();
}

void ELParticleSystem::render() {
    ELParticalSystemCompare compare(this);
    std::sort(activeParticals.begin(),activeParticals.end(),compare);
    this->setNeedRegenData();
    glDisable(GL_CULL_FACE);
    ELGeometry::render();
    glEnable(GL_CULL_FACE);
}