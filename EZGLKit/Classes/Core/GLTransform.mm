//
//  GLTransform.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/8.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "GLTransform.h"

#import <GLKit/GLKit.h>

@implementation GLTransform

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.scaleX = 1.0;
        self.scaleY = 1.0;
        self.scaleZ = 1.0;
        
        self.translateX = 0;
        self.translateY = 0;
        self.translateZ = 0;
        
        self.quaternion = GLKQuaternionIdentity;
        
    }
    return self;
}

- (GLKMatrix4)matrix {
    GLKMatrix4 matrix = GLKMatrix4Identity;
    matrix = GLKMatrix4Translate(matrix,self.translateX, self.translateY, self.translateZ);
    matrix = GLKMatrix4Multiply(matrix,GLKMatrix4MakeWithQuaternion(self.quaternion));
    matrix = GLKMatrix4Scale(matrix, self.scaleX, self.scaleY, self.scaleZ);
    return matrix;
}

@end
