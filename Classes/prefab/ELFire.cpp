//
// Created by wangyang on 16/12/26.
//

#include "ELFire.h"

#include "../core/ELTexture.h"
#include "../core/ELGameObject.h"
#include "../core/ELWorld.h"
#include "../component/ELParticleSystem.h"
#include "../utils/ELAssets.h"

ELFire::ELFire(ELWorld *world, ELVector3 position) {
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("particle_fire.png"))->value;
    ELGameObject *gameObject = new ELGameObject(std::shared_ptr<ELWorld>(world));
    world->addNode(std::shared_ptr<ELGameObject>(gameObject));
    world->addNode(std::shared_ptr<ELFire>(this));
    ELParticleSystem *ps = new ELParticleSystem();
    gameObject->transform->position = position;
    ps->materials[0].diffuseMap = diffuseMap;
    gameObject->addComponent(ps);
    ELParticleSystemData data = ELParticleSystemDataDefault();

    // Fire
    data.maxParticleAmount = 250;
    data.birthRate = 0.1;
    data.force = ELVector3Make(0,0.6,0);
    data.velocity = ELVector3Make(0.0, 1 ,0.0);
    data.velocityRandomRangeBegin = ELVector3Make(-1.6,0,-0.6);
    data.velocityRandomRangeEnd = ELVector3Make(0.6,1,0.6);
    data.positionRandomRangeBegin = ELVector3Make(-0.1,0,-0.1);
    data.positionRandomRangeEnd = ELVector3Make(0.1,0.0,0.1);
    data.sizeBegin = 4.3;
    data.sizeBeginRandomRangeBegin = 0;
    data.sizeBeginRandomRangeEnd = 1.5;
    data.sizeEnd = 1.1;
    data.sizeEndRandomRangeBegin = 0;
    data.sizeEndRandomRangeEnd = 0.2;
    data.colorBegin = ELVector4Make(1.0,0.7,0.1,1.0);
    data.colorBeginRandomRangeBegin = ELVector4Make(0.0,0,0,0.0);
    data.colorBeginRandomRangeEnd = ELVector4Make(0.1,0.2,0,0.0);
    data.colorEnd = ELVector4Make(1.0,0.0,0.0,1);
    data.colorEndRandomRangeBegin = ELVector4Make(0.0,0,0,0.0);
    data.colorEndRandomRangeEnd = ELVector4Make(0.0,0.0,0.0,0.0);
    data.rotationBegin = 190;
    data.rotationBeginRandomRangeBegin = 0;
    data.rotationBeginRandomRangeEnd = 0;
    data.rotationEnd = 90;
    data.rotationEndRandomRangeBegin = 0;
    data.rotationEndRandomRangeEnd = 180;
    data.age = 0.6;
    data.ageRandomBegin = 0;
    data.ageRandomEnd = 1.4;

    data.blendFuncSRC = GL_ONE_MINUS_SRC_ALPHA;
    data.blendFuncDST = GL_DST_ALPHA;

    ps->setData(data);
}