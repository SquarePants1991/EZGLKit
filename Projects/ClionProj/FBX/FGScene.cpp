//
// Created by wangyang on 16/12/2.
//

#include <component/geometry/ELSphereGeometry.h>
#include "FGScene.h"
#include "utils/ELFBXLoader.h"

#define  Bit(n) (0x00000001 << n)

enum CollisionTypes {
    CT_Floor = Bit(0),
    CT_Prop = Bit(1),
    CT_Prop2 = Bit(2),
    CT_Role = Bit(3),
    CT_Prop3 = Bit(4)
};

void FGScene::update(ELFloat timeInSecs) {

}

void FGScene::mouseLeftButtonClicked() {
}

void FGScene::mouseRightButtonClicked() {

}

FGScene::FGScene(ELWorld *world) : world(world){

    ELLight *defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(0,30,30);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    defaultLight->intensityFallOff = 0.0;
    defaultLight->identity = "main-light";
//    defaultLight->enableShadow();
    world->addNode(defaultLight);

    createFloor();
    createMonkey();


    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 3, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
//    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);


    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asSphere(4);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    playerRigidBody = rigidBody;
    rigidBody->collisionGroup = CT_Role;
    rigidBody->collisionMask = CT_Floor;
    gameObject->addComponent(rigidBody);

    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    world->activedCamera->lockOn(gameObject->transform);
}

void FGScene::createPlane() {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("Airbus A310.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("Turkish Airlines.png"))->value;

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
//    gameObject->transform->scale = ELVector3Make(0.02,0.02,0.02);

    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    for (int i = 0; i < geometries.size(); ++i) {
//        geometries.at(i)->material.diffuse = ELVector4Make(0.5,0,0,1);
        geometries.at(i)->materials[0].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[0].diffuseMap = diffuseMap_head;
        geometries.at(i)->materials[1].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[1].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[1].diffuseMap = diffuseMap_body;
        gameObject->addComponent(geometries.at(i));
    }
}

void FGScene::createMonkey() {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,3,0);
    gameObject->transform->scale = ELVector3Make(0.04,0.04,0.04);

    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("humanoid.fbx").c_str());
    for (int i = 0; i < geometries.size(); ++i) {
//        geometries.at(i)->material.diffuse = ELVector4Make(0.5,0,0,1);
        geometries.at(i)->materials[0].diffuse = ELVector4Make(0.6, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[0].diffuseMap = diffuseMap_head;
        geometries.at(i)->materials[1].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[1].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[1].diffuseMap = diffuseMap_body;
        geometries.at(i)->materials[2].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[2].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[2].diffuseMap = diffuseMap_m4;
        gameObject->addComponent(geometries.at(i));
    }
}

void FGScene::createFloor() {
    ELFloat width = 100;
    ELFloat height = 100;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,-10,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("box.png"))->value;

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    createCubeGameObject(ELVector3Make(width,1,height),ELVector3Make(0,0,0),0,diffuseMap,normalMap,false, CT_Floor, CT_Prop2 | CT_Prop | CT_Role, ELVector3Make(0,0,0));
}

void FGScene::createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size, true);
    gameObject->addComponent(cube);
    cube->materials[0].diffuse = ELVector4Make(0.2, 0.2, 0.2, 1.0);
    cube->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
    cube->materials[0].diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->materials[0].normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1, 0, 0, 1);

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(size.x / 2, size.y / 2, size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(velocity);
}