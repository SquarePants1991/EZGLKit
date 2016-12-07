//
// Created by wangyang on 16/12/2.
//

#include "FGScene.h"

FGScene::FGScene(ELWorld *world) : world(world){
    createScene();
}

void FGScene::createScene() {
    // init game world

    ELLight * defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(-8,8,0);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    defaultLight->identity = "main-light";
    defaultLight->enableShadow();
    world->addNode(defaultLight);

//    float size = 14;
//    for (int i=0;i<3;i++) {
//        for (int j=0;j<3;j++) {
//            //createMiddleWalls(ELVector3Make(i * size + 5- 14*3/2.0,0,j * size + 5- 14*3/2.0),5,5);
//            createMiddleWalls(ELVector3Make(i * size - 14*3/2.0,0,j * size- 14*3/2.0),10,10);
//        }
//    }
//
    createBoundWall(ELVector3Make(0,0,0),17 * 3,17 * 3,100);
//
    createFloor();
//
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("tree.png"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("tree.png"))->value;
    for (int i=0;i<20;i++) {
        for (int j=0;j<20;j++) {
            createBoardGameObject(ELVector2Make(2.5,5),ELVector3Make(i * 4 - 14*3/2.0,2.5,j * 4- 14*3/2.0),0,diffuseMap,normalMap);
        }
    }

    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    createCubeGameObject(ELVector3Make(1,1,1),ELVector3Make(0,5,0),10.0,diffuseMap,normalMap);

    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("particle_polygon.png"))->value;
    createParticalGameObject(ELVector2Make(1,1),ELVector3Make(0,-0.2,0),10.0,diffuseMap,normalMap);

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 4.5, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.4,1,0.4));
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);


    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(0.2,1,0.2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    playerRigidBody = rigidBody;
    world->activedCamera->lockOn(gameObject->transform);
}

// 比例关系  1 => 1m
// 人  =>  2 => 2m
// 墙高 =>  3.5 => 3.5m
// 地面长宽  =>  10 X 10 => 10m x 10m

void FGScene::createBoundWall(ELVector3 offset, ELFloat width,ELFloat height,ELFloat wallHeight) {
    ELVector3 xWallsSize = ELVector3Make(0.5,wallHeight,height);
    ELVector3 zWallsSize = ELVector3Make(width,wallHeight,0.5);
    GLuint diffuseMap,normalMap;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    createCubeGameObject(xWallsSize,ELVector3Make(-width / 2 + offset.x,wallHeight / 2.0,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(xWallsSize,ELVector3Make(width / 2 + offset.x,wallHeight / 2.0,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(zWallsSize,ELVector3Make(0+ offset.x,wallHeight / 2.0,-height / 2+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(zWallsSize,ELVector3Make(0+ offset.x,wallHeight / 2.0,height / 2+ offset.z),0.0,diffuseMap,normalMap);
}

void FGScene::createFloor() {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    createCubeGameObject(floorSize,ELVector3Make(0,0,0),0.0,diffuseMap,normalMap);
}

void FGScene::createMiddleWalls(ELVector3 offset,ELFloat width,ELFloat height) {
    ELFloat wallHeight = 3.5;
    ELVector3 xWallsSize = ELVector3Make(0.5,wallHeight,height);
    ELVector3 zWallsSize = ELVector3Make(width,wallHeight,0.5);
    GLuint diffuseMap,normalMap;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    createCubeGameObject(xWallsSize,ELVector3Make(-width / 2+ offset.x,wallHeight / 2.0,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(xWallsSize,ELVector3Make(width / 2+ offset.x,wallHeight / 2.0,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(zWallsSize,ELVector3Make(0+ offset.x,wallHeight / 2.0,-height / 2+ offset.z),0.0,diffuseMap,normalMap);
}

void FGScene::createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size);
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(0.0,0.0,0.0,1.0);
    cube->material.ambient = ELVector4Make(0.4,0.4,0.4,1.0);
    cube->material.diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->material.normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(size.x / 2,size.y / 2,size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,mass);
    gameObject->addComponent(rigidBody);
}

void FGScene::createBoardGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    gameObject->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(3.14/2,0,1,0);
    ELBoard *cube = new ELBoard(size);
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(0.0,0.0,0.0,1.0);
    cube->material.ambient = ELVector4Make(0.4,0.4,0.4,1.0);
    cube->material.diffuseMap = diffuseMap;
    cube->material.normalMap = normalMap;

//    ELCollisionShape *collisionShape = new ELCollisionShape();
//    collisionShape->asBox(ELVector3Make(size.x / 2,size.y / 2,size.x / 2));
//    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,mass);
//    gameObject->addComponent(rigidBody);
}

void FGScene::createParticalGameObject(ELVector2 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap) {
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELParticleSystem *ps = new ELParticleSystem();
    ps->material.diffuseMap = diffuseMap;
    gameObject->addComponent(ps);
    ELParticleSystemData data;

    data.maxParticleAmount = 300;
    data.birthRate = 0.1;
    data.force = ELVector3Make(0,0.3,0);
    data.velocity = ELVector3Make(0.0, 0.5 ,0.0);
    data.velocityRandomRangeBegin = ELVector3Make(-0.0,0,-0.0);
    data.velocityRandomRangeEnd = ELVector3Make(0.0,0.6,0.0);
    data.positionRandomRangeBegin = ELVector3Make(-0.1,0,-0.1);
    data.positionRandomRangeEnd = ELVector3Make(0.1,0.2,0.1);
    data.sizeBegin = 2;
    data.sizeBeginRandomRangeBegin = -1;
    data.sizeBeginRandomRangeEnd = 1;
    data.sizeEnd = 0.4;
    data.sizeEndRandomRangeBegin = -0.2;
    data.sizeEndRandomRangeEnd = 0.2;
    data.colorBegin = ELVector4Make(1.0,0.2,0,1.0);
    data.colorBeginRandomRangeBegin = ELVector4Make(-0.2,0,0,0.0);
    data.colorBeginRandomRangeEnd = ELVector4Make(0.0,0.2,0,0.0);
    data.colorEnd = ELVector4Make(0.2,0.2,0.2,0.8);
    data.colorEndRandomRangeBegin = ELVector4Make(-0.1,0,0,0.0);
    data.colorEndRandomRangeEnd = ELVector4Make(0.0,0.0,0.0,0.0);
    data.age = 3;
    data.ageRandomBegin = 0;
    data.ageRandomEnd = 1;

    //  萤火虫
//    data.force = ELVector3Make(0,0,0);
//    data.velocity = ELVector3Make(0.0, 0.0 ,0.0);
//    data.velocityRandomRangeBegin = ELVector3Make(-0.6,-0.6,-0.6);
//    data.velocityRandomRangeEnd = ELVector3Make(0.6,0.6,0.6);
//    data.positionRandomRangeBegin = ELVector3Make(-30,0,-30);
//    data.positionRandomRangeEnd = ELVector3Make(30,40,30);
//    data.sizeBegin = 1;
//    data.sizeBeginRandomRangeBegin = 0;
//    data.sizeBeginRandomRangeEnd = 1;
//    data.sizeEnd = 0;
//    data.sizeEndRandomRangeBegin = 0;
//    data.sizeEndRandomRangeEnd = 0.2;
//    data.colorBegin = ELVector4Make(1.0,0,0,1.0);
//    data.colorBeginRandomRangeBegin = ELVector4Make(-0.1,0,0,0.0);
//    data.colorBeginRandomRangeEnd = ELVector4Make(0.0,0.3,0,0.0);
//    data.colorEnd = ELVector4Make(1.0,1,0,0.0);
//    data.colorEndRandomRangeBegin = ELVector4Make(-0.1,0,0,0.0);
//    data.colorEndRandomRangeEnd = ELVector4Make(0.0,0.0,0.5,0.0);
//    data.age = 5;
//    data.ageRandomBegin = 0;
//    data.ageRandomEnd = 3;

    ps->setData(data);


}