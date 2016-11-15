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
    CGFloat adjustPercent;
}

@property (strong, nonatomic) EZGLSplineCylinderGeometry *geometry;

@end

@implementation EZGLSplineCylinderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    EZGLPerspectiveCamera *camera = (EZGLPerspectiveCamera *)self.world.camera;
    [camera translateForward:3];
    self.isStickerEnabled = NO;
    
    self.geometry = [[EZGLSplineCylinderGeometry alloc] initWithHeight:5 radius:2 segments:60 ring:60];
    [self.geometry.spline adjust:3 * 5 / 4 offset:-0.1];
    [self.world addGeometry:self.geometry];

    [self.geometry commitChanges];
    
    [self.world setPhysicsEnabled:NO];
}

- (NSString *)shaderName {
    return @"OneLightWithBump";
}

- (void)update {
    [super update];
    self.geometry.transform.quaternion = GLKQuaternionMakeWithAngleAndAxis(radian, 0, 1, 0);
    radian += self.timeSinceLastUpdate * M_PI / 2;
    

    [self.geometry.spline adjust:adjustPercent * 5 offset:-0.1 *self.timeSinceLastUpdate];
    [self.geometry commitChanges];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint pt = [[touches anyObject] locationInView:self.view];
    adjustPercent = pt.y / self.view.bounds.size.height;
}

@end
