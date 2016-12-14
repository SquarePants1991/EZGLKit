//
//  EZGLMoveJoyStickerDelegate.h
//  EZGLKit
//
//  Created by wang yang on 2016/11/13.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EZGLMoveJoySticker;

typedef struct _EZGLMoveJoyStickerState {
    CGFloat offsetX;
    CGFloat offsetY;
    CGFloat deltaOffsetX;
    CGFloat deltaOffsetY;
}EZGLMoveJoyStickerState;

@protocol EZGLMoveJoyStickerDelegate <NSObject>

- (void)joyStickerStateUpdated:(EZGLMoveJoyStickerState)state joySticker:(EZGLMoveJoySticker *)joySticker;

@end
