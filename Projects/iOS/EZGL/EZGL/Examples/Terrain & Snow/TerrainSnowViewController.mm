//
//  TerrainSnowViewController.m
//  EZGL
//
//  Created by wangyang on 2017/2/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "TerrainSnowViewController.h"

#define  Bit(n) (0x00000001 << n)

enum CollisionTypes {
    CT_Floor = Bit(0),
    CT_Prop = Bit(1),
    CT_Prop2 = Bit(2),
    CT_Role = Bit(3),
    CT_Prop3 = Bit(4)
};

@interface TerrainSnowViewController () {
    prop_strong(ELWorld, world);
    ELRigidBody *playerRigidBody;
    ELVector3 walkingFactor;
}
@end

@implementation TerrainSnowViewController

RegisterExample(@"Terrain & Snow")

- (void)viewDidLoad {
    [super viewDidLoad];
    world = retain_ptr(ELWorld, [self world]);
    self.isStickerEnabled = YES;
    
    
    [self createTerrain];
    [self createSnow];
    
    walkingFactor = ELVector3Make(0, 0, 0);
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = ELVector3Make(0, 35, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
    
    
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asSphere(3);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,100.0);
    playerRigidBody = rigidBody;
    rigidBody->collisionGroup = CT_Role;
    rigidBody->collisionMask = CT_Floor;
    gameObject->addComponent(rigidBody);
    
    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    world->activedCamera->lockOn(retain_ptr(ELTransform, gameObject->transform));
}

- (void)createTerrain {
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    printf("%s",gameObject->description().c_str());
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELTerrain *terrain = new ELTerrain(ELVector2Make(200,200),ELAssets::shared()->findFile("island.png"),50);
    
    gameObject->addComponent(terrain);
    GLuint diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("dirt.png"))->value;
    GLuint normalMap = ELTexture::texture(ELAssets::shared()->findFile("grass_green.png"))->value;
    GLuint dudvNormalMap = ELTexture::texture(ELAssets::shared()->findFile("grass_yellow.png"))->value;
    terrain->materials[0].ambient = ELVector4Make(0.3,0.3,0.3,1.0);
    terrain->materials[0].diffuseMap = diffuseMap;
    gameObject->transform->position = ELVector3Make(0,0,0);
    terrain->genMap();
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asTerrian(terrain->mapData,terrain->mapDataSize,0,50);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
    rigidBody->collisionGroup = CT_Floor;
    rigidBody->collisionMask = CT_Prop2 | CT_Prop | CT_Role;
    gameObject->addComponent(rigidBody);
}

- (void)createSnow {
    ELSnow *snow = new ELSnow(world, ELVector3Make(0, 100, 0), ELVector2Make(200, 200));
}

- (void)update {
    EZGLMoveJoyStickerState moveState = self.moveSticker.state;
    walkingFactor.x = moveState.offsetX;
    walkingFactor.z = -moveState.offsetY;
    
    ELVector3 forwardDirection = world->activedCamera->forwardVector();
    forwardDirection = ELVector3MultiplyScalar(forwardDirection,walkingFactor.z);
    ELVector3 leftDirection = world->activedCamera->leftVector();
    leftDirection = ELVector3MultiplyScalar(leftDirection,walkingFactor.x);
    ELVector3 direction = ELVector3Add(forwardDirection, leftDirection);
    playerRigidBody->setVelocityX(direction.x);
    playerRigidBody->setVelocityZ(direction.z);
    [super update];
}

@end
