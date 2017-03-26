//
//  UIImage+GL.h
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/ES2/glext.h>

@interface UIImage (GL)
+ (GLubyte *)dataFromImage:(NSString *)imageName width:(GLsizei *)pWidth height:(GLsizei *)pHeight;
+ (NSArray *)texturesFromGif:(NSString *)gifImage;
+ (GLuint)textureFromCGImage:(CGImageRef)imageRef data:(unsigned char **)data;
+ (GLuint)textureFromCGImage:(CGImageRef)imageRef data:(unsigned char **)data bind:(GLint)texID;
+ (GLuint)textureNamed:(NSString *)imageName;
+ (GLuint)textureFromImageData:(uint8_t *)imgData size:(CGSize)size bind:(GLint)texID pixelFormat:(GLenum)pixelFormat;
+ (GLuint)textureFromImageData:(uint8_t *)imgData size:(CGSize)size bind:(GLint)texID pixelFormat:(GLenum)pixelFormat dataType:(GLenum)dataType;
@end
