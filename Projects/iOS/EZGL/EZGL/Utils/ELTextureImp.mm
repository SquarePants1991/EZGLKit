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

#import "UIImage+GL.h"

@implementation ELTextureImp

GLuint genTexture(const char *path, unsigned char **data, ELInt &width, ELInt &height) {
    NSString *pathStr = [NSString stringWithUTF8String:path];
    UIImage *img = [UIImage imageWithContentsOfFile:pathStr];
    width = [img size].width;
    height = [img size].height;
    return [UIImage textureFromCGImage:img.CGImage data:data];
}

+ (void)load {
    ELTexture::config(genTexture);
}
@end
