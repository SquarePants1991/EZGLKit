//
// Created by wangyang on 16/12/2.
//

#include <component/geometry/ELSphereGeometry.h>
#include <cstdlib>
#include "FGScene.h"

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
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt_01.jpg"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;
    ELVector3 velocity = world->activedCamera->forwardVector();
    velocity = ELVector3MultiplyScalar(velocity, 30);
    createSphereGameObject(ELVector3Make(2,2,2),world->activedCamera->position(),2.0,diffuseMap,normalMap, true, CT_Prop, CT_Role | CT_Prop | CT_Floor,velocity);
}

void FGScene::mouseRightButtonClicked() {

}

FGScene::FGScene(std::shared_ptr<ELWorld> world) : world(world) {
    createScene();
    ELGeometry::resetBorderBeforeUpdate = true;
}

void FGScene::createScene() {
    // init game world

    ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("ArmyPilot.fbx").c_str());

    defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(0,1,1);
    defaultLight->type = ELLightTypeDirection;
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    defaultLight->intensityFallOff = 0.0;
    defaultLight->identity = "main-light";
    defaultLight->enableShadow();
    world->addNode(retain_ptr(ELLight, defaultLight));
    createFloor();
//    createTerrain();
    createPlayer();


    createSphereGameObject(ELVector3Make(5,5,5),world->activedCamera->position(),2.0,1,1, true, CT_Prop, CT_Prop2 | CT_Prop | CT_Floor,ELVector3Make(0,0,0));
    for (int i = 0; i < 360; i += 4) {
        float x = sin(i / 180.0 * 3.14) * 26;
        float z = cos(i / 180.0 * 3.14) * 26;
        float y = rand() / (float) RAND_MAX * 6;
        float size =  rand() / (float) RAND_MAX * 6 + 3;
        createCubeGameObject(ELVector3Make(size, size, size), ELVector3Make(x, y, z), 2.0, ELVector4Make(1,0.7,0.56,1), -1, true, CT_Prop,
                             CT_Floor | CT_Prop2 | CT_Prop | CT_Role);
    }

    createMonkey();
}

void FGScene::createPlayer() {
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 35, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);

    std::shared_ptr<ELCollisionShape> collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asSphere(3);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    playerRigidBody = rigidBody;
    rigidBody->collisionGroup = CT_Role;
    rigidBody->collisionMask = CT_Floor | CT_Prop;
    gameObject->addComponent(rigidBody);

    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    world->activedCamera->lockOn(gameObject->transform);
}

void FGScene::createFloor() {
    ELFloat width = 100 * 6;
    ELFloat height = 100 * 6;
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELGeometry *cube = createCubeGameObject(ELVector3Make(width,10,height),ELVector3Make(0,-25,0),0,ELVector4Make(0.8,0.8,0.6,1),-1,false, CT_Floor, CT_Prop2 | CT_Prop | CT_Role);
    cube->receiveShadow = true;
}

ELGeometry * FGScene::createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,ELVector4 diffuseColor,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size, false);
    gameObject->addComponent(cube);
    cube->materials[0].diffuse = diffuseColor;
    cube->materials[0].ambient = ELVector4Make(0.4, 0.4, 0.4, 1.0);
    cube->materials[0].diffuseMap = -1;
    cube->materials[0].normalMap = 0;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1, 0, 0, 1);

    std::shared_ptr<ELCollisionShape> collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asBox(ELVector3Make(size.x / 2, size.y / 2, size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(velocity);

    return cube;
}

void FGScene::createSphereGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = pos;
    ELSphereGeometry *cube = new ELSphereGeometry(size.x, 20,20);
    gameObject->addComponent(cube);
    cube->materials[0].diffuse = ELVector4Make(0.4,0.8,0.8,1.0);
    cube->materials[0].ambient = ELVector4Make(0.7,0.7,0.7,1.0);
    cube->materials[0].diffuseMap = -1;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->materials[0].normalMap = 0;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1,0,0,1);

    std::shared_ptr<ELCollisionShape> collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asSphere(size.x);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(velocity);
}

void FGScene::createTerrain() {
    ELGameObject *gameObject = new ELGameObject(world);
    printf("%s",gameObject->description().c_str());
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELTerrain *terrain = new ELTerrain(ELVector2Make(150,150),ELAssets::shared()->findFile("island12.png"),24);
    gameObject->addComponent(terrain);
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt_01.jpg"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;
    GLuint dudvNormalMap = ELTexture::texture(ELAssets::shared()->findFile("grass_01.jpeg"))->value;
    terrain->receiveShadow = true;
    terrain->materials[0].ambient = ELVector4Make(0.1,0.1,0.1,1.0);
    terrain->materials[0].diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
    terrain->materials[0].diffuseMap = diffuseMap;
    terrain->materials[0].ambientMap = normalMap;
    terrain->materials[0].specularMap = dudvNormalMap;
    gameObject->transform->position = ELVector3Make(0,0,0);
    terrain->genMap();
    std::shared_ptr<ELCollisionShape> collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asTerrian(terrain->mapData,terrain->mapDataSize,0,24);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
    rigidBody->collisionGroup = CT_Floor;
    rigidBody->collisionMask = CT_Prop2 | CT_Prop | CT_Role;
    gameObject->addComponent(rigidBody);
}

void FGScene::createMonkey() {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELGameObject *gameObject = new ELGameObject(std::shared_ptr<ELWorld>(world));
    world->addNode(std::shared_ptr<ELGameObject>(gameObject));
    gameObject->transform->position = ELVector3Make(0,-20,0);
    gameObject->transform->scale = ELVector3Make(0.1,0.1,0.1);

//    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("humanoid.fbx").c_str());
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        geometries.at(i)->setAnimation((*animations.begin()).second.name);
        geometries.at(i)->materials[0].diffuse = ELVector4Make(0.7,0.4,0,1.0);
        geometries.at(i)->materials[0].normalMap = 0;
        gameObject->addComponent(geometries.at(i));
    }
}