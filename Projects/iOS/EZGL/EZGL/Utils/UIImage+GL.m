//
//  UIImage+GL.m
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "UIImage+GL.h"
#import <ImageIO/ImageIO.h>
#import <GLKit/GLKit.h>

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

+ (GLuint)textureNamed:(NSString *)imageName {
    return [self textureFromCGImage:[UIImage imageNamed:imageName].CGImage data:NULL];
}

+ (GLuint)textureFromImageData:(uint8_t *)imgData size:(CGSize)size bind:(GLint)texID pixelFormat:(GLenum)pixelFormat {
    return [self textureFromImageData:imgData size:size bind:texID pixelFormat:pixelFormat dataType:GL_UNSIGNED_BYTE];
}

+ (GLuint)textureFromImageData:(uint8_t *)imgData size:(CGSize)size bind:(GLint)texID pixelFormat:(GLenum)pixelFormat dataType:(GLenum)dataType {
    
    
    size_t width = size.width;
    size_t height = size.height;
    
    //        width = pow(2,(int)log2(width));
    //        height = pow(2,(int)log2(height));
    
    GLuint texture;
    if (texID < 0) {
        glGenTextures(1, &texture);
    } else {
        texture = texID;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, (GLsizei)width, (GLsizei)height, 0, pixelFormat, dataType, imgData);
    // use linear filetring
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // clamp to edge
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return texture;
}

+ (GLuint)textureFromCGImage:(CGImageRef)imageRef data:(unsigned char **)data bind:(GLint)texID {
    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
    
    //    width = pow(2,(int)log2(width));
    //    height = pow(2,(int)log2(height));
    
    GLubyte *textureData = (GLubyte *)malloc(width * height * 4);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    
    CGContextRef context = CGBitmapContextCreate(textureData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    if (context) {
        CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, height);
        CGContextConcatCTM(context, flipVertical);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    }
    CGContextRelease(context);
    
    GLuint texture;
    if (texID < 0) {
        glGenTextures(1, &texture);
    } else {
        texture = texID;
    }
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (data != NULL) {
        *data = textureData;
    }
    return texture;
}

+ (GLuint)textureFromCGImage:(CGImageRef)imageRef data:(unsigned char **)data {
    return [self textureFromCGImage:imageRef data:data bind:-1];
}

+ (NSArray *)texturesFromGif:(NSString *)gifImage {
    NSString *path = [[NSBundle mainBundle] pathForResource:gifImage ofType:@".gif"];
    if ([[NSFileManager defaultManager] fileExistsAtPath:path] == NO) {
        return [NSArray new];
    }
    
    CGImageSourceRef imageSource = CGImageSourceCreateWithData((__bridge CFDataRef)[NSData dataWithContentsOfFile:path], NULL);
    size_t count = CGImageSourceGetCount(imageSource);
    
    NSMutableArray *textures = [NSMutableArray new];
    
    for (size_t i = 0; i<count; i++) {
        CGImageRef imageRef = CGImageSourceCreateImageAtIndex(imageSource, i, NULL);
        GLuint texture = [self textureFromCGImage:imageRef data:NULL];
        [textures addObject:@(texture)];
        CFRelease(imageRef);
    }
    
    CFRelease(imageSource);
    
    return textures;
}

@end
