//
//  EZGLCubeWithTextureViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/10/27.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLCubeWithTextureViewController.h"
#import <EZGLKit/EZGLKit.h>

@interface EZGLCubeWithTextureViewController ()

@property (strong, nonatomic) EZGLWorld *world;
@property (strong, nonatomic) EZGLWaveFrontGeometry *geometry;

@property (assign, nonatomic) CGPoint lastTouchPoint;
@property (assign, nonatomic) CGFloat lastScale;

@end

@implementation EZGLCubeWithTextureViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.world = [[EZGLWorld alloc] initWithGLKView:(GLKView *)self.view];
    EZGLProgram *program = [[EZGLProgram alloc]initWithVertexShaderFileName:@"WithTexture" fragmentShaderFileName:@"WithTexture"];
    self.world.effect = [[EZGLEffect alloc] initWithProgram:program];
    
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"cube3" ofType:@".obj"];
    self.geometry = [[EZGLWaveFrontGeometry alloc] initWithWaveFrontFilePath:path];
    [self.world addGeometry:self.geometry];
    
    
    UIPinchGestureRecognizer *gesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(panned:)];
    [self.view addGestureRecognizer:gesture];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [self.world render:rect];
}

- (void)update {
    [self.world update:self.timeSinceLastUpdate];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.lastTouchPoint = [touches.anyObject locationInView:self.view];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint pt = [touches.anyObject locationInView:self.view];
    CGFloat dx = pt.x - self.lastTouchPoint.x;
    CGFloat dy = pt.y - self.lastTouchPoint.y;
    
    EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
    [perspectiveCamera rotateEyeWithAngle:-dx / 40.0 axis:perspectiveCamera.up];
    [perspectiveCamera rotateEyeWithAngle:-dy / 40.0 axis:perspectiveCamera.left];
    
    self.lastTouchPoint = pt;
}

- (void)panned:(UIPinchGestureRecognizer *)gesture {
    
    if (gesture.state == UIGestureRecognizerStateBegan) {
        self.lastScale = gesture.scale;
    } else {
        CGFloat scaleDelta = gesture.scale - self.lastScale;
        EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
        [perspectiveCamera translateForward:scaleDelta * 1.6];
        
        self.lastScale = gesture.scale;
    }
}

@end
