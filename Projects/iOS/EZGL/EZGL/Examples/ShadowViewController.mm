//
//  TerrainSnowViewController.m
//  EZGL
//
//  Created by wangyang on 2017/2/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "ShadowViewController.h"

#define  Bit(n) (0x00000001 << n)

enum CollisionTypes {
    CT_Floor = Bit(0),
    CT_Prop = Bit(1),
    CT_Prop2 = Bit(2),
    CT_Role = Bit(3),
    CT_Prop3 = Bit(4)
};

@interface ShadowViewController () {
    prop_strong(ELWorld, world);
    ELRigidBody *playerRigidBody;
    ELVector3 walkingFactor;
}
@end

@implementation ShadowViewController

RegisterExample(@"Shadows")

- (void)viewDidLoad {
    [super viewDidLoad];
    
    world = retain_ptr(ELWorld, [self world]);
    self.isStickerEnabled = YES;
    
    
    [self createFloor];
    [self createCubes];
    
    walkingFactor = ELVector3Make(0, 0, 0);
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(retain_ptr(ELGameObject, gameObject));
    gameObject->transform->position = ELVector3Make(0, 35, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
    
    
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asSphere(10);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,100.0);
    playerRigidBody = rigidBody;
    rigidBody->collisionGroup = CT_Role;
    rigidBody->collisionMask = CT_Floor | CT_Prop;
    gameObject->addComponent(rigidBody);
    
    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
    world->activedCamera->lockOn(retain_ptr(ELTransform, gameObject->transform));
}

- (void)createFloor {
    auto gameObject = retain_ptr_init_v(ELGameObject, world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    ELCubeGeometry *cubeGeometry = new ELCubeGeometry(ELVector3Make(500, 6, 500));
    gameObject->addComponent(cubeGeometry);

    cubeGeometry->materials[0].ambient = ELVector4Make(0.3,0.3,0.3,1.0);
    cubeGeometry->materials[0].diffuse = ELVector4Make(0.6,0.3,0.3,1.0);;
    cubeGeometry->materials[0].diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("island.png"))->value;
    cubeGeometry->receiveShadow = true;
    
    auto collisionShape = retain_ptr_init(ELCollisionShape);
    collisionShape->asBox(ELVector3Make(250, 3, 250));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,0);
    rigidBody->collisionGroup = CT_Floor;
    rigidBody->collisionMask = CT_Prop2 | CT_Prop | CT_Role;
    rigidBody->friction = 0.6;
    gameObject->addComponent(rigidBody);
}

- (void)createCubes {
    for (float i = 0; i < 360; i += 10) {
        float posX = sin(i / 180.0 * M_PI) * 40;
        float posZ = cos(i / 180.0 * M_PI) * 40;
        float size = rand() / (float)RAND_MAX * 3 + 2;
        float posY = rand() / (float)RAND_MAX * 10 + 5;
        
        auto gameObject = retain_ptr_init_v(ELGameObject, world);
        world->addNode(gameObject);
        gameObject->transform->position = ELVector3Make(posX,posY,posZ);
        ELCubeGeometry *cubeGeometry = new ELCubeGeometry(ELVector3Make(size, size, size));
        gameObject->addComponent(cubeGeometry);
        
        cubeGeometry->materials[0].ambient = ELVector4Make(0.3,0.3,0.3,1.0);
        cubeGeometry->materials[0].diffuse = ELVector4Make(0.1,0.8,0.6,1.0);;
        
        
        auto collisionShape = retain_ptr_init(ELCollisionShape);
        collisionShape->asBox(ELVector3Make(size / 2, size / 2, size / 2));
        ELRigidBody *rigidBody = new ELRigidBody(collisionShape,5);
        rigidBody->collisionGroup = CT_Prop;
        rigidBody->collisionMask = CT_Floor | CT_Prop | CT_Role;
        gameObject->addComponent(rigidBody);

    }
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
