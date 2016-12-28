//
// Created by wangyang on 16/12/2.
//

#include <component/geometry/ELSphereGeometry.h>
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
    ELVector3 start = world->activedCamera->position();
    ELVector3 end = ELVector3Add(start,ELVector3MultiplyScalar(world->activedCamera->forwardVector(),100));
    std::vector<ELGameObject *> objs = ELPhysicsWorld::shared()->raycast(start,end,CT_Prop);
    for (int i = 0; i < objs.size(); ++i) {
        ELGameObject *obj = objs.at(i);
        std::vector<ELNode *> nodes = obj->findChildrenWithKind("geometry");
        for (int j = 0; j < nodes.size(); ++j) {
            ELGeometry *geo = (ELGeometry *)nodes.at(j);
            geo->enableBorder = true;
        }
    }
}

void FGScene::mouseLeftButtonClicked() {
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt_01.jpg"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;
    ELVector3 velocity = world->activedCamera->forwardVector();
    velocity = ELVector3MultiplyScalar(velocity, 30);
    createSphereGameObject(ELVector3Make(1,1,1),world->activedCamera->position(),2.0,diffuseMap,normalMap, true, CT_Prop2, CT_Prop2 | CT_Prop | CT_Floor,velocity);
}

void FGScene::mouseRightButtonClicked() {

}

FGScene::FGScene(ELWorld *world) : world(world){
    createScene();
    ELGeometry::resetBorderBeforeUpdate = true;
}

void FGScene::createScene() {
    // init game world

    defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(0,30,30);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    defaultLight->intensityFallOff = 0.0;
    defaultLight->identity = "main-light";
//    defaultLight->enableShadow();
    world->addNode(defaultLight);
    createTerrain();
//    createFloor();
    createWater();
    createSkySphere();
//    new ELFire(world, ELVector3Make(35,10,35));
//    new ELSnow(world, ELVector3Make(0,0,0));
    new ELExplosion(world, ELVector3Make(35,30,35));
    world->addNode(new ELProjector());

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 5, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
//    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);


    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asSphere(3);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    playerRigidBody = rigidBody;
    rigidBody->collisionGroup = CT_Role;
    rigidBody->collisionMask = CT_Floor;
    gameObject->addComponent(rigidBody);

    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    world->activedCamera->lockOn(gameObject->transform);

    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt_01.jpg"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;

    for (int i = 0; i < 10; ++i) {
        srand(rand());
        float x = rand() / (float) RAND_MAX * 6 - 3;
        float z = rand() / (float) RAND_MAX * 16 - 3;
        float y = rand() / (float) RAND_MAX * 6 - 3 + 8;
        createCubeGameObject(ELVector3Make(3, 3, 3), ELVector3Make(x, y, z), 2.0, diffuseMap, normalMap, true, CT_Prop,
                             CT_Floor | CT_Prop2 | CT_Prop);
    }
}

void FGScene::createSkySphere() {
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    ELSkySphere *cube = new ELSkySphere(500);
    cube->renderType = ELGeometryRenderTypeBackSide;
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(0.0,0.0,0.0,1.0);
    cube->material.ambient = ELVector4Make(1.0,1.0,1.0,1.0);
    cube->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("sky.png"))->value;
//    cube->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
}

// 比例关系  1 => 1m
// 人  =>  2 => 2m
// 墙高 =>  3.5 => 3.5m
// 地面长宽  =>  10 X 10 => 10m x 10m

void FGScene::createBoundWall(ELVector3 offset, ELFloat width,ELFloat height,ELFloat wallHeight) {
    ELVector3 xWallsSize = ELVector3Make(0.5,wallHeight,height);
    ELVector3 zWallsSize = ELVector3Make(width,wallHeight,0.5);
    GLuint diffuseMap,normalMap;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("tree.jpg"))->value;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    createCubeGameObject(xWallsSize,ELVector3Make(-width / 2 + offset.x,wallHeight / 2.0-2,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(xWallsSize,ELVector3Make(width / 2 + offset.x,wallHeight / 2.0-2,0.0+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(zWallsSize,ELVector3Make(0+ offset.x,wallHeight / 2.0-2,-height / 2+ offset.z),0.0,diffuseMap,normalMap);
    createCubeGameObject(zWallsSize,ELVector3Make(0+ offset.x,wallHeight / 2.0-2,height / 2+ offset.z),0.0,diffuseMap,normalMap);
}

void FGScene::createFloor() {
    ELFloat width = 1000 * 3;
    ELFloat height = 1000 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,-10,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    createCubeGameObject(ELVector3Make(500,1,500),ELVector3Make(0,100,0),0,diffuseMap,normalMap,false, CT_Floor, CT_Prop2 | CT_Prop | CT_Role);
//
//    ELCollisionShape *collisionShape = new ELCollisionShape();
//    collisionShape->asBox(ELVector3Make(width/2,0.5,height/2));
//    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
//    gameObject->addComponent(rigidBody);
}

void FGScene::createMonkey() {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,3,0);

    std::vector<ELMeshGeometry *> geometries = ELWaveFrontLoader::loadFile(ELAssets::shared()->findFile("monkey.obj"));
    for (int i = 0; i < geometries.size(); ++i) {
        geometries.at(i)->material.diffuseMap = diffuseMap;
        gameObject->addComponent(geometries.at(i));
    }
    
//    ELCollisionShape *collisionShape = new ELCollisionShape();
//    collisionShape->asBox(ELVector3Make(width/2,0.5,height/2));
//    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
//    gameObject->addComponent(rigidBody);
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

void FGScene::createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size, true);
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
    cube->material.ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
    cube->material.diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->material.normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
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

void FGScene::createSphereGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELSphereGeometry *cube = new ELSphereGeometry(1, 20,20);
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(0.0,0.0,0.0,1.0);
    cube->material.ambient = ELVector4Make(0.7,0.7,0.7,1.0);
    cube->material.diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->material.normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1,0,0,1);

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asSphere(1);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(velocity);
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


    //  萤火虫
//    data.maxParticleAmount = 300;
//    data.birthRate = 0.1;
//    data.force = ELVector3Make(0,0,0);
//    data.velocity = ELVector3Make(0.0, 0.0 ,0.0);
//    data.velocityRandomRangeBegin = ELVector3Make(-0.6,-0.6,-0.6);
//    data.velocityRandomRangeEnd = ELVector3Make(0.6,0.6,0.6);
//    data.positionRandomRangeBegin = ELVector3Make(-30,0,-30);
//    data.positionRandomRangeEnd = ELVector3Make(30,40,30);
//    data.sizeBegin = 5;
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
//    data.rotationBegin = 230;
//    data.rotationBeginRandomRangeBegin = 0;
//    data.rotationBeginRandomRangeEnd = 10;
//    data.rotationEnd = 90;
//    data.rotationEndRandomRangeBegin = 0;
//    data.rotationEndRandomRangeEnd = 180;

}

void FGScene::createWater() {
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELWaterPlane *waterPlane = new ELWaterPlane(ELVector2Make(1000,1000));
    gameObject->addComponent(waterPlane);

    gameObject->specificEffectName = "water";
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;
    GLuint dudvNormalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal_dvdu.png"))->value;
    waterPlane->material.diffuse = ELVector4Make(0.0,0.0,1.0,1.0);
    waterPlane->dudvMap = dudvNormalMap;
}

void FGScene::createTerrain() {
    ELGameObject *gameObject = new ELGameObject(world);
    printf("%s",gameObject->description().c_str());
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELTerrain *terrain = new ELTerrain(ELVector2Make(200,200),ELAssets::shared()->findFile("island6.png"),70);
    gameObject->addComponent(terrain);
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt_01.jpg"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("water_normal.png"))->value;
    GLuint dudvNormalMap = ELTexture::texture(ELAssets::shared()->findFile("grass_01.jpeg"))->value;
    terrain->material.ambient = ELVector4Make(0.3,0.3,0.3,1.0);
    terrain->material.diffuseMap = diffuseMap;
    terrain->material.ambientMap = normalMap;
    terrain->material.specularMap = dudvNormalMap;
    gameObject->transform->position = ELVector3Make(0,0,0);
    terrain->genMap();
    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asTerrian(terrain->mapData,terrain->mapDataSize,0,70);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
    rigidBody->collisionGroup = CT_Floor;
    rigidBody->collisionMask = CT_Prop2 | CT_Prop | CT_Role;
    gameObject->addComponent(rigidBody);
}