//
//  EZGLSplineCylinderViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/14.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLSplineCylinderViewController.h"
#import <EZGLKit/EZGLSplineCylinderGeometry.h>

@interface EZGLSplineCylinderViewController ()

@end

@implementation EZGLSplineCylinderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    EZGLSplineCylinderGeometry *cylinder = [[EZGLSplineCylinderGeometry alloc] initWithHeight:5 radius:2 segments:40 ring:50];
    [self.world addGeometry:cylinder];
    
    [self.world setPhysicsEnabled:NO];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}

@end
