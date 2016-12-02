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

    float size = 14;
    for (int i=0;i<3;i++) {
        for (int j=0;j<3;j++) {
            createMiddleWalls(ELVector3Make(i * size + 5,0,j * size + 5),5,5);
            createMiddleWalls(ELVector3Make(i * size,0,j * size),10,10);
        }
    }

    createBoundWall(ELVector3Make(0,0,0),17 * 3,17 * 3);

    createFloor();

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 4.5, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.4,2,0.4));
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
//    cube->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
//    cube->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

//    player = gameObject;

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(0.2,2,0.2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    playerRigidBody = rigidBody;
//
//
//    ELGameObject *gameObject2 = new ELGameObject(world);
//    world->addNode(gameObject2);
//    gameObject2->transform->position = ELVector3Make(0, 0, 0);
//    ELCubeGeometry *plane = new ELCubeGeometry(ELVector3Make(80,1,80));
//    gameObject2->addComponent(plane);
//    plane->material.diffuse = ELVector4Make(1.0,1.0,1.0,1.0);
////   plane->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
//    plane->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
//
//    ELCollisionShape *collisionShape2 = new ELCollisionShape();
//    collisionShape2->asBox(ELVector3Make(40,0.5,40));
//    ELRigidBody *rigidBody2 = new ELRigidBody(collisionShape2,0.0);
//    gameObject2->addComponent(rigidBody2);
//
////    ELGameObject *gameObject3 = new ELGameObject(world);
////    world->addNode(gameObject3);
////    gameObject3->transform->position = ELVector3Make(0, 0, -4);
////    gameObject3->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2,1,0,0);
////    ELPlaneGeometry *plane2 = new ELPlaneGeometry(ELVector2Make(50,50));
////    gameObject3->addComponent(plane);
////    gameObject3->addComponent(effect);
////    plane2->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
//
////    std::vector<ELMeshGeometry *> geometries =  ELWaveFrontLoader::loadFile(ELAssets::shared()->findFile("scene3.obj"));
////    for (int i = 0; i < geometries.size(); ++i) {
////        ELGameObject *gameObjectMesh = new ELGameObject(world);
////        world->addNode(gameObjectMesh);
////        gameObjectMesh->addComponent(geometries.at(i));
//////        gameObjectMesh->addComponent(effect);
////        gameObjectMesh->transform->position = ELVector3Make( 0 , 0 , 0);
////        geometries.at(i)->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
////        geometries.at(i)->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
////    }
//
    world->activedCamera->lockOn(gameObject->transform);
}

// 比例关系  1 => 1m
// 人  =>  2 => 2m
// 墙高 =>  3.5 => 3.5m
// 地面长宽  =>  10 X 10 => 10m x 10m

void FGScene::createBoundWall(ELVector3 offset, ELFloat width,ELFloat height) {
    ELFloat wallHeight = 3.5;
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
    cube->material.diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->material.normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(size.x / 2,size.y / 2,size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,mass);
    gameObject->addComponent(rigidBody);
}