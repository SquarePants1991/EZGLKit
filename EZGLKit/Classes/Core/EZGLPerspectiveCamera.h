//
//  EZGLCamera.h
//  Pods
//
//  Created by wang yang on 2016/10/21.
//
//

#import <Foundation/Foundation.h>
#import "EZGLTransform.h"

@interface EZGLPerspectiveCamera : NSObject

@property (assign, nonatomic) GLfloat nearZ;
@property (assign, nonatomic) GLfloat farZ;
@property (assign, nonatomic) GLfloat fovyRadians;
@property (assign, nonatomic) GLfloat aspect;

@property (assign, nonatomic) GLKVector3 lookAt;
@property (assign, nonatomic) GLKVector3 up;
@property (strong, nonatomic) EZGLTransform *transform;

@end
