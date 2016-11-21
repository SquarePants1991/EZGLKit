//
//  EZGLBaseViewController.h
//  EZGLKit
//
//  Created by wangyang on 2016/11/11.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <EZGLKit/EZGLKit.h>
#import "EZGLMoveJoySticker.h"

@interface EZGLBaseViewController : GLKViewController

@property (strong, nonatomic) EZGLWorld *world;
@property (assign, nonatomic) BOOL isStickerEnabled;

@property (strong, nonatomic) EZGLMoveJoySticker *moveSticker;
@property (strong, nonatomic) EZGLMoveJoySticker *rotateSticker;

// override this
- (NSString *)shaderName;
- (void)update;

@end
