//
//  EZGLSimplePhysicsViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/13.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLSimplePhysicsViewController.h"
#import <EZGLKit/EZGLPlaneGeometry.h>
#import <EZGLKit/EZGLCylinderGeometry.h>
#import <EZGLKit/EZGLSphereGeometry.h>
#import <EZGLKit/EZGLConeGeometry.h>
#import <EZGLKit/EZGLCubeGeometry.h>

@interface EZGLSimplePhysicsViewController ()

@end

@implementation EZGLSimplePhysicsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"cube3" ofType:@".obj"];
    EZGLWaveFrontGeometry *geometry = [[EZGLWaveFrontGeometry alloc] initWithWaveFrontFilePath:path];
    geometry.transform.translateY = 0;
    [self.world addGeometry:geometry];
    
    EZGLCylinderGeometry *cylinder = [[EZGLCylinderGeometry alloc] initWithHeight:5 radius:2 segments:40];
    [cylinder.transform setTranslateX: 2];
    [cylinder.transform setQuaternion:GLKQuaternionMakeWithAngleAndAxis(M_PI / 2, 0, 0, 1)];
    [self.world addGeometry:cylinder];
    
    EZGLSphereGeometry *sphere = [[EZGLSphereGeometry alloc] initWithRadius:3 segments:40 ring:30];
    [sphere.transform setTranslateX: -2];
    [sphere.transform setTranslateY: 10];
    [self.world addGeometry:sphere];
    
    EZGLConeGeometry *cone = [[EZGLConeGeometry alloc] initWithRadius:2 segments:40 height:5];
    [cone.transform setTranslateX: -2];
    [cone.transform setTranslateY: 6];
    [self.world addGeometry:cone];
    
    EZGLCubeGeometry *cube = [[EZGLCubeGeometry alloc] initWithSize:GLKVector3Make(3, 2, 6)];
    [cube.transform setTranslateX: -5];
    [cube.transform setTranslateY: 6];
    [self.world addGeometry:cube];
    
    EZGLPlaneGeometry *planeGeo = [[EZGLPlaneGeometry alloc] initWithSize:CGSizeMake(100, 100)];
    [planeGeo.transform setTranslateY: -5];
    [self.world addGeometry:planeGeo];
    
    [self.world setPhysicsEnabled:YES];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}


@end
