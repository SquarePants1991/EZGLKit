//
//  EZGLSplineCylinderViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/14.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLSplineCylinderViewController.h"
#import <EZGLKit/EZGLSplineCylinderGeometry.h>
#import <EZGLKit/EZGLCylinderGeometry.h>

@interface EZGLSplineCylinderViewController () {
    CGFloat radian;
    CGFloat adjustPercent;
    CGFloat adjustOffset;
}

@property (strong, nonatomic) EZGLSplineCylinderGeometry *geometry;
    @property (strong, nonatomic) EZGLCylinderGeometry *baseCylinder;

@end

@implementation EZGLSplineCylinderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    adjustPercent = -1;
    adjustOffset = 0;
    
    EZGLLight *secondLight = [[EZGLLight alloc] init];
    secondLight.color = GLKVector4Make(1.0, 1.0, 1.0, 1.0);
    secondLight.position = GLKVector3Make(-20, 0, 5);
    
    [self.world.effect addLight:secondLight];
//
//    EZGLLight *thirdLight = [[EZGLLight alloc] init];
//    thirdLight.color = GLKVector4Make(1.0, 1.0, 1.0, 1.0);
//    thirdLight.position = GLKVector3Make(0, 0, 10);
//    
//    [self.world.effect addLight:thirdLight];
    
    EZGLPerspectiveCamera *camera = (EZGLPerspectiveCamera *)self.world.camera;
    [camera translateForward:3];
    self.isStickerEnabled = NO;
    
    self.geometry = [[EZGLSplineCylinderGeometry alloc] initWithHeight:5 radius:2 segments:25 ring:25];
    [self.world addGeometry:self.geometry];
    
    self.baseCylinder = [[EZGLCylinderGeometry alloc] initWithHeight:1 radius:5 segments:25];
    self.baseCylinder.transform.translateY = -3;
    [self.world addGeometry:self.baseCylinder];

    [self.geometry commitChanges];
    
    [self.world setPhysicsEnabled:NO];
}

- (NSString *)shaderName {
    return @"MultiLightWithBumpEdge";
}

- (void)update {
    [super update];
    self.geometry.transform.quaternion = GLKQuaternionMakeWithAngleAndAxis(radian, 0, 1, 0);
    self.baseCylinder.transform.quaternion = GLKQuaternionMakeWithAngleAndAxis(radian, 0, 1, 0);
    radian += self.timeSinceLastUpdate * M_PI / 2;
    
    if (adjustPercent > 0) {
    [self.geometry.spline adjust:adjustPercent * 5 offset:adjustOffset *self.timeSinceLastUpdate];
    [self.geometry commitChanges];
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint pt = [[touches anyObject] locationInView:self.view];
    if (pt.x < self.view.bounds.size.width / 2) {
        adjustPercent = 1 - pt.y / self.view.bounds.size.height;
        adjustOffset = 0.5;
    } else {
        adjustPercent = 1 - pt.y / self.view.bounds.size.height;
        adjustOffset = -0.5;
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    adjustPercent = -1;
    adjustOffset = 0;
}
    
@end
