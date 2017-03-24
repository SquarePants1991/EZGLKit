//
//  EZGLBaseViewController.h
//  EZGLKit
//
//  Created by wangyang on 2016/11/11.
//  Copyright © 2016年 ocean. All rights reserved.
//

#import <GLKit/GLKit.h>
#import "EZGL.h"
#import "EZGLMoveJoySticker.h"
#import "ExampleCollector.h"

@interface EZGLBaseViewController : GLKViewController

@property (assign, nonatomic) BOOL isStickerEnabled;

@property (strong, nonatomic) EZGLMoveJoySticker *moveSticker;
@property (strong, nonatomic) EZGLMoveJoySticker *rotateSticker;

// override this
- (NSString *)shaderName;
- (void)update;
- (ELWorld *)world;

@end
