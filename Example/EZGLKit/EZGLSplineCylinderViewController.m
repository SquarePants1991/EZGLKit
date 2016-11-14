//
//  EZGLSplineCylinderViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/14.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLSplineCylinderViewController.h"
#import <EZGLKit/EZGLSplineCylinderGeometry.h>

@interface EZGLSplineCylinderViewController () {
    CGFloat radian;
}

@property (strong, nonatomic) EZGLSplineCylinderGeometry *geometry;

@end

@implementation EZGLSplineCylinderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.geometry = [[EZGLSplineCylinderGeometry alloc] initWithHeight:5 radius:2 segments:40 ring:50];
    [self.world addGeometry:self.geometry];
    
    [self.world setPhysicsEnabled:NO];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}

- (void)update {
    self.geometry.transform.quaternion = GLKQuaternionMakeWithAngleAndAxis(radian, 0, 1, 0);
    radian += self.timeSinceLastUpdate * M_PI / 2;
}

@end
