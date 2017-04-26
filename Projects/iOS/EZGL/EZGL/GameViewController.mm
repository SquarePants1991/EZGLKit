//
//  GameViewController.m
//  EZGL
//
//  Created by wangyang on 2017/1/19.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import <EZGLib/EZGL.h>

#define  Bit(n) (0x00000001 << n)

enum CollisionTypes {
    CT_Floor = Bit(0),
    CT_Prop = Bit(1),
    CT_Prop2 = Bit(2),
    CT_Role = Bit(3),
    CT_Prop3 = Bit(4)
};

@interface GameViewController () {
    prop_strong(ELWorld, world);
    prop_strong(ELGameObject, tank);
    prop_strong(ELGameObject, floor);
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

@end

@implementation GameViewController

RegisterExample(@"Knight")

ELGameObject * createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);

- (void)viewDidLoad
{
    [super viewDidLoad];
    world = retain_ptr(ELWorld, [self world]);
    world->activedCamera->perspective(ELVector3Make(0, 100, 400), ELVector3Make(0, 0, 0), ELVector3Make(0, 1, 0), world->activedCamera->fovyRadians, world->activedCamera->aspect, world->activedCamera->nearZ, world->activedCamera->farZ);
    self.isStickerEnabled = YES;
    
    
    [self createFloor];
    [self createACube];
    [self createArmy];
    
//    ELGameObject *gameObject = new ELGameObject(world);
//    world->addNode(gameObject);
//    gameObject->transform->position = ELVector3Make(0, 3, 0);
//    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
//    //    gameObject->addComponent(cube);
//    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
//    
//    
//    ELCollisionShape *collisionShape = new ELCollisionShape();
//    collisionShape->asSphere(4);
//    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
////    playerRigidBody = rigidBody;
//    rigidBody->collisionGroup = CT_Role;
//    rigidBody->collisionMask = CT_Floor;
//    gameObject->addComponent(rigidBody);
    
//    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
//    world->activedCamera->lockOn(gameObject->transform);
}

- (void)createFloor {
    ELFloat width = 140;
    ELFloat height = 140;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,-10,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("stone_ground_normal.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("stone_ground.png"))->value;
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    floor = createCubeGameObject(world, ELVector3Make(width,50,height),ELVector3Make(0,-25,0),0,diffuseMap,normalMap,false, CT_Floor, CT_Prop2 | CT_Prop | CT_Role, ELVector3Make(0,0,0));
}

- (void)createACube {
    ELFloat width = 1;
    ELFloat height = 3;
    ELVector3 floorSize = ELVector3Make(width,3,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    createCubeGameObject(world, ELVector3Make(width,width,width),ELVector3Make(0,10,0),2,diffuseMap,normalMap,false, CT_Prop, CT_Prop2 | CT_Floor | CT_Role | CT_Prop, ELVector3Make(0,0,0));
}

- (void)createArmy {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    tank = gameObject;
    world->addNode(gameObject);
    static float posY = 0;
    gameObject->transform->position = ELVector3Make(posY,0,0);
    
    posY += 200;
    //gameObject->transform->scale = ELVector3Make(0.2,0.2,0.2);
//    gameObject->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2, 0, 1, 0);
    
    //        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("draw_sword_2_1.fbx").c_str());
    ELVector3 finalSize = ELVector3Make(0, 0, 0);
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        auto iter = animations.begin();
        iter ++;
        geometries.at(i)->setAnimation((*iter).second.name);
        geometries.at(i)->generateData();
        gameObject->addComponent(geometries.at(i));
        
        if (finalSize.x < geometries.at(i)->size.x) finalSize.x = geometries.at(i)->size.x;
        if (finalSize.y < geometries.at(i)->size.y) finalSize.y = geometries.at(i)->size.y;
        if (finalSize.z < geometries.at(i)->size.z) finalSize.z = geometries.at(i)->size.z;
    }
    
    finalSize = ELVector3MultiplyScalar(finalSize, 0.005);
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asBox(ELVector3Make(finalSize.x / 2, finalSize.y / 2, finalSize.z / 2));
    collisionShape->offset = ELVector3Make(0, -finalSize.y / 2, 0);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, 100);
    rigidBody->collisionGroup = CT_Prop;
    rigidBody->collisionMask = CT_Floor | CT_Prop | CT_Prop2 | CT_Prop3;
    rigidBody->friction = 0.5;
//    gameObject->addComponent(rigidBody);
}

- (void)createTank {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    tank = gameObject;
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    gameObject->transform->scale = ELVector3Make(0.005,0.005,0.005);
    gameObject->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2, 0, 1, 0);
    
    //        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("ausfb.fbx").c_str());
    ELVector3 finalSize = ELVector3Make(0, 0, 0);
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        geometries.at(i)->setAnimation("hello");
        geometries.at(i)->generateData();
        gameObject->addComponent(geometries.at(i));
        
        if (finalSize.x < geometries.at(i)->size.x) finalSize.x = geometries.at(i)->size.x;
        if (finalSize.y < geometries.at(i)->size.y) finalSize.y = geometries.at(i)->size.y;
        if (finalSize.z < geometries.at(i)->size.z) finalSize.z = geometries.at(i)->size.z;
    }
    
    finalSize = ELVector3MultiplyScalar(finalSize, 0.005);
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asBox(ELVector3Make(finalSize.x / 2, finalSize.y / 2, finalSize.z / 2));
    collisionShape->offset = ELVector3Make(0, -finalSize.y / 2, 0);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, 100);
    rigidBody->collisionGroup = CT_Prop;
    rigidBody->collisionMask = CT_Floor | CT_Prop | CT_Prop2 | CT_Prop3;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
}

- (void)createMonkey {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    gameObject->transform->scale = ELVector3Make(0.1,0.1,0.1);
    
//        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("humanoid.fbx").c_str());
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        geometries.at(i)->setAnimation((*animations.begin()).second.name);
        geometries.at(i)->generateData();
        geometries.at(i)->enableBorder = true;
        geometries.at(i)->borderWidth = 0.2;
        geometries.at(i)->borderColor = ELVector4Make(0.0, 1.0, 0.81, 1.0);
        float sizeY = 0;//geometries.at(i)->size.y;
        gameObject->transform->position = ELVector3Make(0,sizeY * 0.1 / 2,0);
        geometries.at(i)->materials[0].diffuse = ELVector4Make(0.6, 1.0, 1.0, 1.0);
        geometries.at(i)->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[0].diffuseMap = diffuseMap;
        geometries.at(i)->materials[1].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[1].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[1].diffuseMap = diffuseMap_body;
        geometries.at(i)->materials[2].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[2].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[2].diffuseMap = diffuseMap_m4;
        gameObject->addComponent(geometries.at(i));
    }
}

std::shared_ptr<ELGameObject> createCubeGameObject(std::shared_ptr<ELWorld> world, ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {
    
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size, false);
    gameObject->addComponent(cube);
    cube->materials[0].diffuse = ELVector4Make(0.3, 0.3, 0.3, 1.0);
    cube->materials[0].ambient = ELVector4Make(0.3,0.3,0.3, 1.0);
    cube->materials[0].diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->materials[0].normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1, 0, 0, 1);
    
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asBox(ELVector3Make(size.x / 2, size.y / 2, size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
//    gameObject->addComponent(rigidBody);
//    rigidBody->setVelocity(velocity);
    
    return gameObject;
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}


    - (void)userScaled:(float)deltaScale {
        tank->transform->scale = ELVector3AddScalar( tank->transform->scale, deltaScale);
    }
    
    - (void)userMoved:(CGPoint)deltaPosition {
        tank->transform->quaternion = ELQuaternionMultiply(tank->transform->quaternion, ELQuaternionMakeWithAngleAndAxis(deltaPosition.x / 10.0, 0, 1, 0));
           floor->transform->quaternion = ELQuaternionMultiply(floor->transform->quaternion, ELQuaternionMakeWithAngleAndAxis(deltaPosition.x / 10.0, 0, 1, 0));
    }
@end
