//
//  EZGLCamera.m
//  Pods
//
//  Created by wang yang on 2016/10/21.
//
//

#import "EZGLPerspectiveCamera.h"
#import <GLKit/GLKit.h>

@interface EZGLPerspectiveCamera ()

@property (assign, nonatomic) CGFloat degreeAroundLeft;
@property (assign, nonatomic) CGFloat degreeAroundUp;
@property (assign, nonatomic) CGFloat degreeAroundForward;

@property (assign, nonatomic) GLKVector3 translation;

@end

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

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.degreeAroundUp = 0.0;
        self.degreeAroundLeft = 0.0;
        self.degreeAroundForward = 0.0;
    }
    return self;
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

- (GLKQuaternion)quaternion {
    GLKQuaternion forwardQuaternion = GLKQuaternionMakeWithAngleAndAxis(self.degreeAroundForward / 180.0 * M_PI, self.forward.x, self.forward.y, self.forward.z);
    GLKQuaternion upQuaternion = GLKQuaternionMakeWithAngleAndAxis(self.degreeAroundUp / 180.0 * M_PI, self.up.x, self.up.y, self.up.z);
    GLKQuaternion leftQuaternion = GLKQuaternionMakeWithAngleAndAxis(self.degreeAroundLeft / 180.0 * M_PI, self.left.x, self.left.y, self.left.z);
    return GLKQuaternionMultiply(GLKQuaternionMultiply(forwardQuaternion, upQuaternion),leftQuaternion);
}

- (void)rotateEyeWithAngle:(GLfloat)radians axis:(GLKVector3)axis {
    [self rotateWithAngle:radians axis:axis];
}

- (void)rotateLookAtWithAngle:(GLfloat)radians axis:(GLKVector3)axis {
    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndVector3Axis(radians, axis);
    self.up = GLKQuaternionRotateVector3(quaternion,self.up);
    GLKVector3 lookAtVec = GLKVector3Subtract(self.lookAt, self.eye);
    GLKVector3 newLookAtVec = GLKQuaternionRotateVector3(quaternion, lookAtVec);
    self.lookAt = GLKVector3Add(self.eye, newLookAtVec);
}

- (void)rotateLookAtWithAngleAroundUp:(GLfloat)radians {
    self.degreeAroundUp += radians * 180.0 / M_PI;
}

- (void)rotateLookAtWithAngleAroundLeft:(GLfloat)radians {
    self.degreeAroundLeft += radians * 180.0 / M_PI;
}

- (void)rotateWithAngle:(GLfloat)radians axis:(GLKVector3)axis {
    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndVector3Axis(radians, axis);
    self.up = GLKQuaternionRotateVector3(quaternion,self.up);
    GLKVector3 eyeVec = GLKVector3Subtract(self.eye, self.lookAt);
    GLKVector3 newEyeVec = GLKQuaternionRotateVector3(quaternion, eyeVec);
    self.eye = GLKVector3Add(self.lookAt, newEyeVec);
}

- (void)translateForward:(GLfloat)distance {
    GLKQuaternion cameraQuaternion = [self quaternion];
    GLKVector3 transformedForward = GLKQuaternionRotateVector3(cameraQuaternion, self.forward);
    self.translation = GLKVector3Add(self.translation, GLKVector3Make(transformedForward.x * distance, transformedForward.y * distance, transformedForward.z * distance));
}

- (void)translateLeft:(GLfloat)distance {
    GLKQuaternion cameraQuaternion = [self quaternion];
    GLKVector3 transformedLeft = GLKQuaternionRotateVector3(cameraQuaternion, self.left);
    self.translation = GLKVector3Add(self.translation, GLKVector3Make(transformedLeft.x * distance, transformedLeft.y * distance, transformedLeft.z * distance));
}

- (void)translateUp:(GLfloat)distance {
    self.translation = GLKVector3Add(self.translation, GLKVector3Make(self.up.x * distance, self.up.y * distance, self.up.z * distance));
}

- (GLKMatrix4)matrix {
    GLKMatrix4 perspective = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(self.fovyRadians), self.aspect, self.nearZ, self.farZ);
    GLKQuaternion cameraQuaternion = [self quaternion];
    GLKVector3 transformedUp = GLKQuaternionRotateVector3(cameraQuaternion, self.up);
    GLKVector3 transformedForward = GLKQuaternionRotateVector3(cameraQuaternion, self.forward);
    
    GLKVector3 transformedEye = GLKVector3Add(self.eye, self.translation);
    GLKVector3 transformedLookAt = GLKVector3Add(transformedEye, transformedForward);
    
    GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(transformedEye.x,transformedEye.y,transformedEye.z,transformedLookAt.x, transformedLookAt.y, transformedLookAt.z, transformedUp.x, transformedUp.y, transformedUp.z);
    return GLKMatrix4Multiply(perspective, lookAt);
}

@end
