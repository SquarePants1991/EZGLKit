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

- (instancetype)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

+ (EZGLMaterial *)defaultMaterial {
    EZGLMaterial *material = [EZGLMaterial new];
    material.ambient = GLKVector4Make(0.3, 0.3, 0.3, 1.0);
    material.diffuse = GLKVector4Make(0.0, 0.0, 0.0, 1.0);
    material.specular = GLKVector4Make(1.0, 1.0, 1.0, 1.0);
    
    material.ambientMap = [UIImage textureFromCGImage:[UIImage imageNamed:@"default_ambient.png"].CGImage];
    material.normalMap = [UIImage textureFromCGImage:[UIImage imageNamed:@"default_normal.png"].CGImage];
    NSString *image = [[NSBundle bundleForClass:self] pathForResource:@"default" ofType:@".png"];
    material.diffuseMap = [UIImage textureFromCGImage:[UIImage imageWithContentsOfFile:image].CGImage];
    material.specularMap = [UIImage textureFromCGImage:[UIImage imageNamed:@"default_spec.png"].CGImage];
    return material;
}

@end
