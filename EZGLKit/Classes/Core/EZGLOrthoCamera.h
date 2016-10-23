//
//  EZGLOrthCamera.h
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import "EZGLCamera.h"

@interface EZGLOrthoCamera : EZGLCamera

@property (assign, nonatomic) GLfloat nearZ;
@property (assign, nonatomic) GLfloat farZ;

@property (assign, nonatomic) GLfloat width;
@property (assign, nonatomic) GLfloat height;

+ (EZGLCamera *)cameraWithSize:(CGSize)size;

@end
