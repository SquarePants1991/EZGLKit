//
// Created by wangyang on 16/12/6.
//

#include <types/ELGeometryTypes.h>
#include "ELParticleSystem.h"
#include "ELPartical.h"
#include "core/ELGameObject.h"
#include "ELGeometry.h"

#include <functional>
#include <stdlib.h>

class ELParticalSystemCompare {
public:
    ELParticleSystem * system;
    ELParticalSystemCompare(ELParticleSystem *system) {
        this->system = system;
    }
    bool operator()(ELPartical *obj1,ELPartical *obj2)
    {
        ELFloat distance1 = ELVector2Distance(obj1->transform.position.xz,this->system->gameObject()->mainCamera()->position().xz);
        ELFloat distance2 = ELVector2Distance(obj2->transform.position.xz,this->system->gameObject()->mainCamera()->position().xz);
        return distance1 > distance2;
    }
};

ELParticleSystem::ELParticleSystem() : isDataInitialized(false), partical(NULL) {
    emitTimeInterval = 0.3;
    onlyUseColorAttrib = true;
    identity = "psystem";
    isTransparency = true;
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
        emitTimeInterval = 0.04;
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

ELFloat randomFloat(ELFloat initial,ELFloat rangeBegin,ELFloat rangeEnd) {
    srand(rand());
    ELFloat val = initial + rangeBegin + (rangeEnd - rangeBegin) * (rand() / (ELFloat)RAND_MAX);
    return val;
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
    srand(rand());

    partical->transform.position = randomVec3(gameObject()->transform->position,data.positionRandomRangeBegin,data.positionRandomRangeEnd);
    partical->velocity = randomVec3(data.velocity,data.velocityRandomRangeBegin,data.velocityRandomRangeEnd);
    partical->sizeStart = randomVec2Lock(ELVector2Make(data.sizeBegin,data.sizeBegin),data.sizeBeginRandomRangeBegin,data.sizeBeginRandomRangeEnd);
    partical->sizeEnd = randomVec2Lock(ELVector2Make(data.sizeEnd,data.sizeEnd),data.sizeEndRandomRangeBegin,data.sizeEndRandomRangeEnd);
    partical->colorStart = randomVec4(data.colorBegin,data.colorBeginRandomRangeBegin,data.colorBeginRandomRangeEnd);
    partical->colorEnd = randomVec4(data.colorEnd,data.colorEndRandomRangeBegin,data.colorEndRandomRangeEnd);
    partical->rotationStart = randomFloat(data.rotationBegin,data.rotationBeginRandomRangeBegin,data.rotationBeginRandomRangeEnd);
    partical->rotationEnd = randomFloat(data.rotationEnd,data.rotationEndRandomRangeBegin,data.rotationEndRandomRangeEnd);
    partical->age = data.age + data.ageRandomBegin + ( data.ageRandomEnd -  data.ageRandomBegin) * (rand() / (ELFloat)RAND_MAX);
    partical->reset();
}

void ELParticleSystem::render() {
    this->setNeedRegenData();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA);
    ELGeometry::render();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

std::string ELParticleSystem::kind() {
    return "particle_system";
}

ELMatrix4 ELParticleSystem::modelMatrix() {
    return ELMatrix4Identity;
}