//
//  EZGLCubeWithTextureViewController.m
//  EZGLKit
//
//  Created by wang yang on 2016/10/27.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLBaseViewController.h"
#import "EZGLMoveJoySticker.h"

@interface EZGLBaseViewController () <EZGLMoveJoyStickerDelegate>

@property (assign, nonatomic) CGPoint lastTouchPoint;
@property (assign, nonatomic) CGPoint currentTouchPoint;
@property (assign, nonatomic) CGFloat lastScale;

@property (strong, nonatomic) EZGLMoveJoySticker *moveSticker;
@property (strong, nonatomic) EZGLMoveJoySticker *rotateSticker;
@end

@implementation EZGLBaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.world = [[EZGLWorld alloc] initWithGLKView:(GLKView *)self.view];
    EZGLProgram *program = [[EZGLProgram alloc]initWithVertexShaderFileName:[self shaderName] fragmentShaderFileName:[self shaderName]];
    self.world.effect = [[EZGLEffect alloc] initWithProgram:program];
    EZGLLight *defaultLight = [EZGLLight new];
    defaultLight.color = GLKVector4Make(1.0, 1.0, 1.0, 1);
    defaultLight.position = GLKVector3Make(33, 10, 4);
    [self.world.effect addLight:defaultLight];
    
    
    CGRect bounds = self.view.bounds;
    self.moveSticker = [[EZGLMoveJoySticker alloc]initWithFrame:CGRectMake(0, 0, bounds.size.width / 2, bounds.size.height)];
    self.moveSticker.delegate = self;
    [self.view addSubview:self.moveSticker];
    
    self.rotateSticker = [[EZGLMoveJoySticker alloc]initWithFrame:CGRectMake(bounds.size.width / 2, 0, bounds.size.width / 2,bounds.size.height)];
    self.rotateSticker.delegate = self;
    [self.view addSubview:self.rotateSticker];
}

- (void)setIsStickerEnabled:(BOOL)isStickerEnabled {
    _isStickerEnabled = isStickerEnabled;
    if (_isStickerEnabled == NO) {
        self.moveSticker.hidden = YES;
        self.rotateSticker.hidden = YES;
    }
}

- (NSString *)shaderName {
    return @"default";
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [self.world render:rect];
}

- (void)update {
    if (self.isStickerEnabled) {
        [self updateCamera:self.timeSinceLastUpdate];
    }

    [self.world update:self.timeSinceLastUpdate];
}

- (void)joyStickerStateUpdated:(EZGLMoveJoyStickerState)state joySticker:(EZGLMoveJoySticker *)joySticker {
    if (self.isStickerEnabled == NO) {
        return;
    }
    if (joySticker == self.rotateSticker) {
        EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
        [perspectiveCamera rotateLookAtWithAngleAroundUp:-state.deltaOffsetX / 35.0];
        [perspectiveCamera rotateLookAtWithAngleAroundLeft:-state.deltaOffsetY / 35.0];
    }
}

- (void)updateCamera:(NSTimeInterval)interval {
    EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
    
    EZGLMoveJoyStickerState moveState = self.moveSticker.state;
    
    [perspectiveCamera translateForward: -moveState.offsetY / 10 * interval];
    [perspectiveCamera translateLeft: moveState.offsetX / 10 * interval];
    
}

//- (void)panned:(UIPinchGestureRecognizer *)gesture {
//
//    if (gesture.state == UIGestureRecognizerStateBegan) {
//        self.lastScale = gesture.scale;
//    } else {
//        CGFloat scaleDelta = gesture.scale - self.lastScale;
//        EZGLPerspectiveCamera *perspectiveCamera = (EZGLPerspectiveCamera *)self.world.camera;
//        [perspectiveCamera translateForward:scaleDelta * 1.6];
//
//        self.lastScale = gesture.scale;
//    }
//}

@end
