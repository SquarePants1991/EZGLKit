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

@interface EZGLTerrianViewController ()

@end

@implementation EZGLTerrianViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [((EZGLPerspectiveCamera *)self.world.camera) setEye:GLKVector3Make(0, 3, 10)];
    
    EZGLTerrain *terrian = [[EZGLTerrain alloc] initWithImage:nil size:CGSizeMake(100, 100)];
    [self.world addGeometry:terrian];
    
    EZGLCylinderGeometry *baseCylinder = [[EZGLCylinderGeometry alloc] initWithHeight:1 radius:5 segments:25];
    baseCylinder.transform.translateY = -3;
    [self.world addGeometry:baseCylinder];
    
    [self.world setPhysicsEnabled:NO];
    self.isStickerEnabled = YES;
}

- (NSString *)shaderName {
    return @"MultiLightWithBump";
}

@end
