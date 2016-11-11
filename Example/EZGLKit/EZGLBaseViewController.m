//
//  EZGLCubeWithTextureViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/10/27.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLBaseViewController.h"

@interface EZGLBaseViewController ()

@property (assign, nonatomic) CGPoint lastTouchPoint;
@property (assign, nonatomic) CGPoint currentTouchPoint;
@property (assign, nonatomic) CGFloat lastScale;

@property (assign, nonatomic) BOOL isWalkMode;

@end

@implementation EZGLBaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.world = [[EZGLWorld alloc] initWithGLKView:(GLKView *)self.view];
    EZGLProgram *program = [[EZGLProgram alloc]initWithVertexShaderFileName:[self shaderName] fragmentShaderFileName:[self shaderName]];
    self.world.effect = [[EZGLEffect alloc] initWithProgram:program];
    [self.world.effect addLight:[EZGLLight new]];
    
    UIPinchGestureRecognizer *gesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(panned:)];
    [self.view addGestureRecognizer:gesture];
}

- (NSString *)shaderName {
    return @"default";
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [self.world render:rect];
}

- (void)update {
    EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
    if (self.isWalkMode) {
        if (self.currentTouchPoint.y < self.lastTouchPoint.y) {
            [perspectiveCamera translateForward:self.timeSinceLastUpdate * 5];
        } else {
            [perspectiveCamera translateForward:-self.timeSinceLastUpdate * 5];
        }
    }
    
    [self.world update:self.timeSinceLastUpdate];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.lastTouchPoint = [touches.anyObject locationInView:self.view];
    self.isWalkMode = self.lastTouchPoint.x < self.view.frame.size.width / 2;
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint pt = [touches.anyObject locationInView:self.view];
    EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
    if (self.isWalkMode) {
        self.currentTouchPoint = pt;
    } else {
        CGFloat dx = pt.x - self.lastTouchPoint.x;
        CGFloat dy = pt.y - self.lastTouchPoint.y;
        
        
//        [perspectiveCamera rotateEyeWithAngle:-dx / 40.0 axis:perspectiveCamera.up];
//        [perspectiveCamera rotateEyeWithAngle:-dy / 40.0 axis:perspectiveCamera.left];
        
        
        
                [perspectiveCamera rotateLookAtWithAngleAroundUp:-dx / 40.0];
                [perspectiveCamera rotateLookAtWithAngleAroundLeft:-dy / 40.0];
        
        self.lastTouchPoint = pt;
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.isWalkMode = NO;
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
