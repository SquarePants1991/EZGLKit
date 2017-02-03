//
// Created by wangyang on 16/12/27.
//

#include "ELSnow.h"

#include "core/ELTexture.h"
#include "core/ELGameObject.h"
#include "core/ELWorld.h"
#include "component/ELParticleSystem.h"
#include "utils/ELAssets.h"

ELSnow::ELSnow(ELWorld *world, ELVector3 position) {
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("particle_point.png"))->value;
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    world->addNode(this);
    ELParticleSystem *ps = new ELParticleSystem();
    gameObject->transform->position = position;
    ps->materials[0].diffuseMap = diffuseMap;
    gameObject->addComponent(ps);
    ELParticleSystemData data = ELParticleSystemDataDefault();

    data.maxParticleAmount = 1000;
    data.birthRate = 0.01;
    data.force = ELVector3Make(4,-6.5,0);
    data.velocity = ELVector3Make(0.0, -10 ,0.0);
    data.velocityRandomRangeBegin = ELVector3Make(-0.6,0,-0.6);
    data.velocityRandomRangeEnd = ELVector3Make(0.6,-3,0.6);
    data.positionRandomRangeBegin = ELVector3Make(-200,200,-200);
    data.positionRandomRangeEnd = ELVector3Make(200,280,200);
    data.sizeBegin = 1.0;
    data.sizeBeginRandomRangeBegin = 0;
    data.sizeBeginRandomRangeEnd = 2;
    data.sizeEnd = 0.8;
    data.sizeEndRandomRangeBegin = 0;
    data.sizeEndRandomRangeEnd = 1.5;
    data.colorBegin = ELVector4Make(1.0,1.0,1.0,1.0);
    data.colorBeginRandomRangeBegin = ELVector4Make(0.0,0,0,0.0);
    data.colorBeginRandomRangeEnd = ELVector4Make(0.0,0.0,0,0.0);
    data.colorEnd = ELVector4Make(1.0,1.0,1.0,1);
    data.colorEndRandomRangeBegin = ELVector4Make(0.0,0,0,0.0);
    data.colorEndRandomRangeEnd = ELVector4Make(0.0,0.0,0.0,0.0);
    data.rotationBegin = 0;
    data.rotationBeginRandomRangeBegin = 0;
    data.rotationBeginRandomRangeEnd = 0;
    data.rotationEnd = 0;
    data.rotationEndRandomRangeBegin = 0;
    data.rotationEndRandomRangeEnd = 0;
    data.age = 6;
    data.ageRandomBegin = 0;
    data.ageRandomEnd = 1.4;

    data.blendFuncSRC = GL_SRC_ALPHA;
    data.blendFuncDST = GL_DST_ALPHA;

    ps->setData(data);
}