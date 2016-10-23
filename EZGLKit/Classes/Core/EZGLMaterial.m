//
//  EZGLMaterial.m
//  OpenESRD
//
//  Created by wang yang on 2016/10/12.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLMaterial.h"
#import "UIImage+GL.h"

@implementation EZGLMaterial

+ (EZGLMaterial *)defaultMaterial {
    EZGLMaterial *material = [EZGLMaterial new];
    material.ambient = GLKVector4Make(0.6, 0.6, 0.6, 1.0);
    material.diffuse = GLKVector4Make(0.0, 0.0, 0.0, 1.0);
    material.specular = GLKVector4Make(1.0, 1.0, 1.0, 1.0);
    
    
    NSArray *images = [[NSBundle bundleForClass:self] pathsForResourcesOfType:@".jpg" inDirectory:@"./"];
    int index = (int)floor(rand() / (float)RAND_MAX * (images.count - 1));
    NSString *image = images[index];
    material.diffuseMap = [UIImage textureFromCGImage:[UIImage imageWithContentsOfFile:image].CGImage];
    return material;
}

@end
