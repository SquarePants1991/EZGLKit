//
//  EZGLCubeWithTextureViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/10/27.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLCubeWithBumpTextureViewController.h"
#import <EZGLKit/EZGLKit.h>

@interface EZGLCubeWithBumpTextureViewController ()

@property (strong, nonatomic) EZGLWaveFrontGeometry *geometry;

@end

@implementation EZGLCubeWithBumpTextureViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"cube3" ofType:@".obj"];
    self.geometry = [[EZGLWaveFrontGeometry alloc] initWithWaveFrontFilePath:path];
    [self.world addGeometry:self.geometry];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}

@end
