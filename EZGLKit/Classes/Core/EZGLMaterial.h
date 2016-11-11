//
//  EZGLMaterial.h
//  OpenESRD
//
//  Created by wang yang on 2016/10/12.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <GLKit/GLKit.h>

@interface EZGLMaterial : NSObject

@property (assign, nonatomic) GLKVector4 ambient;
@property (assign, nonatomic) GLKVector4 diffuse;
@property (assign, nonatomic) GLKVector4 specular;

@property (assign, nonatomic) GLuint ambientMap;
@property (assign, nonatomic) GLuint diffuseMap;
@property (assign, nonatomic) GLuint normalMap;
@property (assign, nonatomic) GLuint specularMap;

@property (assign, nonatomic) GLuint shadowMap;

+ (EZGLMaterial *)defaultMaterial;
@end
