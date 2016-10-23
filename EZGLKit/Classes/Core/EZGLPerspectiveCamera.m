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

+ (EZGLCamera *)cameraWithSize:(CGSize)size {
    EZGLPerspectiveCamera *camera = [EZGLPerspectiveCamera new];
    camera.aspect = fabs(size.width / size.height);
    camera.fovyRadians = 65.0f;
    camera.nearZ = 0.1;
    camera.farZ = 100;
    camera.lookAt = GLKVector3Make(0, 0, 0);
    camera.up = GLKVector3Make(0, 1, 0);
    camera.eye = GLKVector3Make(0, 0, 10);
    camera.transform = [EZGLTransform new];
    camera.transform.translateY = 10;
    camera.transform.translateZ = 10;
    return camera;
}

- (GLKVector3)left {
    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndVector3Axis(M_PI / 2.0, self.forward);
    GLKVector3 left = GLKQuaternionRotateVector3(quaternion, self.up);
    return left;
}

- (GLKVector3)forward {
    GLKVector3 forward = GLKVector3Normalize(GLKVector3Subtract(self.lookAt, self.eye));
    return forward;
}

- (void)rotateWithAngle:(GLfloat)radians axis:(GLKVector3)axis {
    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndVector3Axis(radians, axis);
    self.up = GLKQuaternionRotateVector3(quaternion,self.up);
    GLKVector3 eyeVec = GLKVector3Subtract(self.eye, self.lookAt);
    GLKVector3 newEyeVec = GLKQuaternionRotateVector3(quaternion, eyeVec);
    self.eye = GLKVector3Add(self.lookAt, newEyeVec);
}

- (void)translateForward:(GLfloat)distance {
    GLKVector3 translateVector = GLKVector3MultiplyScalar(self.forward, distance);
    self.eye = GLKVector3Add(self.eye, translateVector);
}

- (void)translateLeft:(GLfloat)distance {
    GLKVector3 translateVector = GLKVector3MultiplyScalar(self.left, distance);
    self.eye = GLKVector3Add(self.eye, translateVector);
}

- (void)translateUp:(GLfloat)distance {
    GLKVector3 translateVector = GLKVector3MultiplyScalar(self.up, distance);
    self.eye = GLKVector3Add(self.eye, translateVector);
}

- (GLKMatrix4)matrix {
    GLKMatrix4 perspective = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(self.fovyRadians), self.aspect, self.nearZ, self.farZ);
    GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(self.eye.x,self.eye.y,self.eye.z,self.lookAt.x, self.lookAt.y, self.lookAt.z, self.up.x, self.up.y, self.up.z);
    return GLKMatrix4Multiply(perspective, lookAt);
}

@end
