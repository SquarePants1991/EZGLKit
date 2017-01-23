//
//  ELTextureImp.m
//  EZGL
//
//  Created by wangyang on 2017/1/20.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "ELTextureImp.h"
#import <OpenGLES/ES2/glext.h>
#include <EZGL.h>

#import "Utils/UIImage+GL.h"

@implementation ELTextureImp

GLuint genTexture(const char *path) {
    NSString *pathStr = [NSString stringWithUTF8String:path];
    UIImage *img = [UIImage imageWithContentsOfFile:pathStr];
    return [UIImage textureFromCGImage:img.CGImage];
}

+ (void)load {
    ELTexture::config(genTexture);
}
@end
