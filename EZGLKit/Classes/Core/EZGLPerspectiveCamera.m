//
//  EZGLCamera.m
//  Pods
//
//  Created by wang yang on 2016/10/21.
//
//

#import "EZGLPerspectiveCamera.h"
#import <GLKit/GLKit.h>

@implementation EZGLPerspectiveCamera

- (GLKMatrix4)matrix {
    GLKMatrix4 perspective = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(self.fovyRadians), self.aspect, self.nearZ, self.farZ);
    GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(self.lookAt.x, self.lookAt.y, self.lookAt.z, 0, 0, 0, self.up.x, self.up.y, self.up.z);
    return GLKMatrix4Multiply(perspective, lookAt);
}

@end
