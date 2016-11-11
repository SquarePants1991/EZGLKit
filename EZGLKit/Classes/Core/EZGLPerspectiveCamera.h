//
//  EZGLCamera.h
//  Pods
//
//  Created by wang yang on 2016/10/21.
//
//

#import "EZGLCamera.h"

@interface EZGLPerspectiveCamera : EZGLCamera

@property (assign, nonatomic) GLfloat nearZ;
@property (assign, nonatomic) GLfloat farZ;
@property (assign, nonatomic) GLfloat fovyRadians;
@property (assign, nonatomic) GLfloat aspect;

@property (assign, nonatomic) GLKVector3 eye;
@property (assign, nonatomic) GLKVector3 lookAt;
@property (assign, nonatomic) GLKVector3 up;
@property (assign, nonatomic) GLKVector3 left;
@property (assign, nonatomic) GLKVector3 forward;

+ (EZGLCamera *)cameraWithSize:(CGSize)size;

- (void)rotateEyeWithAngle:(GLfloat)radians axis:(GLKVector3)axis;
- (void)rotateLookAtWithAngle:(GLfloat)radians axis:(GLKVector3)axis;
- (void)rotateLookAtWithAngleAroundUp:(GLfloat)radians;
- (void)rotateLookAtWithAngleAroundLeft:(GLfloat)radians;
- (void)translateForward:(GLfloat)distance;
- (void)translateLeft:(GLfloat)distance;
- (void)translateUp:(GLfloat)distance;

@end
