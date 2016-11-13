//
//  EZGLMoveJoySticker.m
//  EZGLKit
//
//  Created by wang yang on 2016/11/13.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import "EZGLMoveJoySticker.h"

@interface EZGLMoveJoySticker ()

@property (assign, nonatomic) CGPoint originTouchPoint;

@end

@implementation EZGLMoveJoySticker

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.originTouchPoint = [touches.anyObject locationInView:self];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint pt = [touches.anyObject locationInView:self];
    CGFloat dx = pt.x - self.originTouchPoint.x;
    CGFloat dy = pt.y - self.originTouchPoint.y;
    if (fabs(dx) > 50) {
        dx = dx / fabs(dx) * 50;
    }
    if (fabs(dy) > 50) {
        dy = dy / fabs(dy) * 50;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(joyStickerStateUpdated:joySticker:)]) {
        EZGLMoveJoyStickerState state;
        state.offsetX = dx;
        state.offsetY = dy;
        self.state = state;
        [self.delegate joyStickerStateUpdated:state joySticker:self];
    }
    
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    EZGLMoveJoyStickerState state;
    state.offsetX = 0;
    state.offsetY = 0;
    self.state = state;
    [self.delegate joyStickerStateUpdated:state joySticker:self];
}

@end
