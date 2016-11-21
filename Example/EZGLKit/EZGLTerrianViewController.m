//
//  EZGLTerrianViewController.m
//  EZGLKit
//
//  Created by wangyang on 2016/11/17.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLTerrianViewController.h"
#import "EZGLMoveJoySticker.h"

#import <EZGLKit/EZGLTerrain.h>
#import <EZGLKit/EZGLCylinderGeometry.h>
#import <EZGLKit/EZGLSphereGeometry.h>
#import <EZGLKit/EZGLSkySphereGeometry.h>

#import <EZGLKit/EZGLFirstPersonView.h>

@interface EZGLTerrianViewController () {
    EZGLTerrain *terrian;
    EZGLFirstPersonView *firstPersonView;
    EZGLCylinderGeometry *person;
}

@end

@implementation EZGLTerrianViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [((EZGLPerspectiveCamera *)self.world.camera) setEye:GLKVector3Make(0, 3, 10)];
    
    terrian = [[EZGLTerrain alloc] initWithImage:[UIImage imageNamed:@"island2.jpeg"] size:CGSizeMake(200, 200)];
    [self.world addNode:terrian];
    
    person = [[EZGLCylinderGeometry alloc] initWithHeight:2 radius:2 segments:25];
    person.transform.translateY = 5;
    [self.world addNode:person];
    
    EZGLSkySphereGeometry *skySphere = [[EZGLSkySphereGeometry alloc] initWithRadius:128 segments:20 ring:20];
    [self.world addNode:skySphere];
    
    
    for (int i=0; i<4;i++) {
    EZGLSphereGeometry *sphere = [[EZGLSphereGeometry alloc] initWithRadius:2 segments:20 ring:20];
    sphere.transform.translateY = 10;
    sphere.transform.translateX = 4 * i - 8;
    sphere.transform.translateZ = 5;
    [self.world addNode:sphere];
    }
    
    [self.world setPhysicsEnabled:YES];
    self.isStickerEnabled = YES;
    
    firstPersonView = [[EZGLFirstPersonView alloc]initWithCamera:((EZGLPerspectiveCamera *)self.world.camera) node:person];
    [self.world addNode:firstPersonView];
}

- (NSString *)shaderName {
    return @"MultiLightWithBump";
}

- (void)update {
    [super update];
}

- (void)joyStickerStateUpdated:(EZGLMoveJoyStickerState)state joySticker:(EZGLMoveJoySticker *)joySticker {
    if (self.isStickerEnabled == NO) {
        return;
    }
    if (joySticker == self.rotateSticker) {
        EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
        [perspectiveCamera rotateLookAtWithAngleAroundUp:-state.deltaOffsetX / 35.0];
        [perspectiveCamera rotateLookAtWithAngleAroundLeft:-state.deltaOffsetY / 35.0];
    }
}

- (void)updateCamera:(NSTimeInterval)interval {
    EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
    
    EZGLMoveJoyStickerState moveState = self.moveSticker.state;
    
    [perspectiveCamera translateForward: -moveState.offsetY / 10 * interval];
    [perspectiveCamera translateLeft: moveState.offsetX / 10 * interval];
    
}

@end
