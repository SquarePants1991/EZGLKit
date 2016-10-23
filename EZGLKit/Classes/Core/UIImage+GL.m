//
//  UIImage+GL.m
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "UIImage+GL.h"
#import <ImageIO/ImageIO.h>

@implementation UIImage (GL)

+ (GLubyte *)dataFromImage:(NSString *)imageName width:(GLsizei *)pWidth height:(GLsizei *)pHeight {
    UIImage *img = [UIImage imageNamed:imageName];
    CGImageRef imageRef = [img CGImage];
    *pWidth = CGImageGetWidth(imageRef);
    *pHeight = CGImageGetHeight(imageRef);
    
    size_t width = *pWidth;
    size_t height = *pHeight;
    
    GLubyte *textureData = (GLubyte *)malloc(width * height * 4);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    
    CGContextRef context = CGBitmapContextCreate(textureData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    return textureData;
}

+ (GLuint)textureFromCGImage:(CGImageRef)imageRef {
    size_t width = 128;//CGImageGetWidth(imageRef);
    size_t height = 128;//CGImageGetHeight(imageRef);
    
    GLubyte *textureData = (GLubyte *)malloc(width * height * 4);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    
    CGContextRef context = CGBitmapContextCreate(textureData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

+ (NSArray *)texturesFromGif:(NSString *)gifImage {
    NSString *path = [[NSBundle mainBundle] pathForResource:gifImage ofType:@".gif"];
    CGImageSourceRef imageSource = CGImageSourceCreateWithData((__bridge CFDataRef)[NSData dataWithContentsOfFile:path], NULL);
    size_t count = CGImageSourceGetCount(imageSource);
    
    NSMutableArray *textures = [NSMutableArray new];
    
    for (size_t i = 0; i<count; i++) {
        CGImageRef imageRef = CGImageSourceCreateImageAtIndex(imageSource, i, NULL);
        GLuint texture = [self textureFromCGImage:imageRef];
        [textures addObject:@(texture)];
        CFRelease(imageRef);
    }
    
    CFRelease(imageSource);
    
    return textures;
}

@end
