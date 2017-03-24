//
//  EZGLMoveJoySticker.h
//  EZGLKit
//
//  Created by wang yang on 2016/11/13.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EZGLMoveJoyStickerDelegate.h"

@interface EZGLMoveJoySticker : UIView

@property (assign, nonatomic) EZGLMoveJoyStickerState state;
@property (weak, nonatomic) id<EZGLMoveJoyStickerDelegate> delegate;

@end
