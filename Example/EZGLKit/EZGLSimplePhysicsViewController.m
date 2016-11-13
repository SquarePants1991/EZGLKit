//
//  EZGLSimplePhysicsViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/13.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLSimplePhysicsViewController.h"
#import <EZGLKit/EZGLPlaneGeometry.h>

@interface EZGLSimplePhysicsViewController ()

@end

@implementation EZGLSimplePhysicsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"cube3" ofType:@".obj"];
    EZGLWaveFrontGeometry *geometry = [[EZGLWaveFrontGeometry alloc] initWithWaveFrontFilePath:path];
    geometry.transform.translateY = 7;
    [self.world addGeometry:geometry];
    
    EZGLPlaneGeometry *planeGeo = [EZGLPlaneGeometry new];
    [planeGeo.transform setTranslateY: -10];
    [self.world addGeometry:planeGeo];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}


@end
