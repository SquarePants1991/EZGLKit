//
//  EZGLLight.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/13.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <GLKit/GLKit.h>
#import "EZGLPerspectiveCamera.h"

@interface EZGLLight : NSObject

@property (assign, nonatomic) GLKVector4 color;
@property (assign, nonatomic) GLfloat intensity;
@property (assign, nonatomic) GLKVector3 position;

- (void)update:(NSTimeInterval)interval;
- (EZGLPerspectiveCamera *)lightCameraWithSize:(CGSize)size;
- (GLfloat *)raw;
- (GLsizei)rawLen;

@end
