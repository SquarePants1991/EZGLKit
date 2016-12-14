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
@property (assign, nonatomic) CGPoint lastTouchPoint;

@end

@implementation EZGLMoveJoySticker

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    self.originTouchPoint = [touches.anyObject locationInView:self];
    self.lastTouchPoint = self.originTouchPoint;
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
        state.deltaOffsetX = pt.x - self.lastTouchPoint.x;
        state.deltaOffsetY = pt.y - self.lastTouchPoint.y;
        self.state = state;
        [self.delegate joyStickerStateUpdated:state joySticker:self];
    }
    self.lastTouchPoint = pt;
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    EZGLMoveJoyStickerState state;
    state.offsetX = 0;
    state.offsetY = 0;
    state.deltaOffsetX = 0;
    state.deltaOffsetY = 0;
    self.state = state;
    [self.delegate joyStickerStateUpdated:state joySticker:self];
}

@end
