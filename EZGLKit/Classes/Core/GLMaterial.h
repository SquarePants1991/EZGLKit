//
//  GLMaterial.h
//  OpenESRD
//
//  Created by wang yang on 2016/10/12.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <GLKit/GLKit.h>

@interface GLMaterial : NSObject

@property (assign, nonatomic) GLKVector4 ambient;
@property (assign, nonatomic) GLKVector4 diffuse;
@property (assign, nonatomic) GLKVector4 specular;
@property (assign, nonatomic) GLuint diffuseMap;
@property (assign, nonatomic) GLuint shadowMap;

@end
