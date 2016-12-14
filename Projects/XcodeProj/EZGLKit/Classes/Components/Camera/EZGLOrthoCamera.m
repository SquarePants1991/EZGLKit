//
//  EZGLOrthCamera.m
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import "EZGLOrthoCamera.h"

@implementation EZGLOrthoCamera

+ (EZGLCamera *)cameraWithSize:(CGSize)size {
    EZGLOrthoCamera *camera = [EZGLOrthoCamera new];
    camera.nearZ = -1;
    camera.farZ = 1000;
    camera.width = size.width;
    camera.height = size.height;
    camera.transform = [EZGLTransform new];
    camera.transform.translateZ = -10;
    return camera;
}

- (GLKMatrix4)matrix {
    GLKMatrix4 ortho = GLKMatrix4MakeOrtho(-self.width / 2, self.width / 2, -self.height / 2, self.height / 2, self.nearZ, self.farZ);
    return GLKMatrix4Multiply(ortho, [self.transform matrix]);
}

@end
