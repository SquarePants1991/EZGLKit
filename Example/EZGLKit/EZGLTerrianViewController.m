//
//  EZGLTerrianViewController.m
//  EZGLKit
//
//  Created by wangyang on 2016/11/17.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLTerrianViewController.h"
#import <EZGLKit/EZGLTerrain.h>
#import <EZGLKit/EZGLCylinderGeometry.h>
#import <EZGLKit/EZGLSphereGeometry.h>
#import <EZGLKit/EZGLSkySphereGeometry.h>

@interface EZGLTerrianViewController () {
    EZGLTerrain *terrian;
}

@end

@implementation EZGLTerrianViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [((EZGLPerspectiveCamera *)self.world.camera) setEye:GLKVector3Make(0, 3, 10)];
    
    terrian = [[EZGLTerrain alloc] initWithImage:[UIImage imageNamed:@"island2.jpeg"] size:CGSizeMake(200, 200)];
    [self.world addGeometry:terrian];
    
    EZGLCylinderGeometry *baseCylinder = [[EZGLCylinderGeometry alloc] initWithHeight:1 radius:1 segments:25];
    baseCylinder.transform.translateY = 5;
    [self.world addGeometry:baseCylinder];
    
    EZGLSkySphereGeometry *skySphere = [[EZGLSkySphereGeometry alloc] initWithRadius:128 segments:20 ring:20];
    [self.world addGeometry:skySphere];
    
    
    for (int i=0; i<4;i++) {
    EZGLSphereGeometry *sphere = [[EZGLSphereGeometry alloc] initWithRadius:2 segments:20 ring:20];
    sphere.transform.translateY = 10;
    sphere.transform.translateX = 4 * i - 8;
    sphere.transform.translateZ = 5;
    [self.world addGeometry:sphere];
    }
    
    [self.world setPhysicsEnabled:YES];
    self.isStickerEnabled = YES;
}

- (NSString *)shaderName {
    return @"MultiLightWithBump";
}

- (void)update {
    [super update];
}

@end
