//
//  OpenGLUtil.h
//  OpenESRD
//
//  Created by wang yang on 2016/10/7.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/glext.h>

#import "EZGLDefines.h"

@interface EZGLProgram : NSObject

@property (assign, nonatomic) GLuint value;

- (instancetype)initWithVertexShaderFileName:(NSString *)vsName fragmentShaderFileName:(NSString *)fsName;
- (instancetype)initWithVertexShaderFile:(NSString *)vsf fragmentShaderFile:(NSString *)fsf;
- (instancetype)initWithVertexShader:(NSString *)vs fragmentShader:(NSString *)fs;

- (int)uniform:(int)uniformName;
@end
