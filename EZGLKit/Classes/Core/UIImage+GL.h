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
+ (GLuint)textureFromCGImage:(CGImageRef)imageRef;
@end
