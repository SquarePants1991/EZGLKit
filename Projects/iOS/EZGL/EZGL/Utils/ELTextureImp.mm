//
//  ELTextureImp.m
//  EZGL
//
//  Created by wangyang on 2017/1/20.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "ELTextureImp.h"
#import <OpenGLES/ES2/glext.h>
#if Use_EZGLFramework
#import <EZGLib/EZGL.h>
#else
#import <EZGL.h>
#endif

#import "UIImage+GL.h"

@implementation ELTextureImp

GLuint genTexture(const char *path, unsigned char **data, ELInt &width, ELInt &height) {
    NSString *pathStr = [NSString stringWithUTF8String:path];
    UIImage *img = [UIImage imageWithContentsOfFile:pathStr];
    width = [img size].width;
    height = [img size].height;
    return [UIImage textureFromCGImage:img.CGImage data:data];
}

void resetTexture(uint8_t *imageData, ELInt texID, ELInt width, ELInt height, GLenum pixelFormat, GLenum dataType) {
    [UIImage textureFromImageData:imageData size:CGSizeMake(width , height) bind:texID pixelFormat:pixelFormat dataType:dataType];
}

+ (void)load {
    ELTexture::config(genTexture, resetTexture);
}
@end
