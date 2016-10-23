//
//  GLTransform.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/8.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface GLTransform : NSObject

@property (assign, nonatomic) GLfloat translateX;
@property (assign, nonatomic) GLfloat translateY;
@property (assign, nonatomic) GLfloat translateZ;

@property (assign, nonatomic) GLfloat scaleX;
@property (assign, nonatomic) GLfloat scaleY;
@property (assign, nonatomic) GLfloat scaleZ;

@property (assign, nonatomic) GLKQuaternion quaternion;

- (GLKMatrix4)matrix;
- (void)applyOpenGLMatrix:(GLfloat *)openGLMatrix; 

@end
