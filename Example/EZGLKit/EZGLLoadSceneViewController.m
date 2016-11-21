//
//  EZGLLoadSceneViewController.m
//  EZGLKit
//
//  Created by wangyang on 2016/11/11.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLLoadSceneViewController.h"

@interface EZGLLoadSceneViewController () {
    EZGLWaveFrontGeometry *geometry;
}
@end

@implementation EZGLLoadSceneViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"demo1_000001" ofType:@".obj"];
    geometry = [[EZGLWaveFrontGeometry alloc] initWithWaveFrontFilePath:path];
    [self.world addNode:geometry];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}


@end
