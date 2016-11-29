//
// Created by wangyang on 16/11/24.
//

#include "ELCamera.h"

ELCamera::ELCamera() {

}

ELCamera::ELCamera(ELVector3 eye,ELVector3 lookAt,ELVector3 up,ELFloat fovyRadians,ELFloat aspect,ELFloat nearZ,ELFloat farZ) :
        eye(eye),
        lookAt(lookAt),
        up(up),
        fovyRadians(fovyRadians),
        aspect(aspect),
        nearZ(nearZ),
        farZ(farZ),
        lockOnTransform(NULL)
{
    translation = ELVector3Make(0, 0, 0);
    radiansAroundForward = 0.0;
    radiansAroundUp = 0.0;
    radiansAroundLeft = 0.0;
}

ELMatrix4 ELCamera::matrix() {
    ELVector3 forward = forwardVector();
    ELVector3 left = leftVector();
    ELMatrix4 perspective = ELMatrix4MakePerspective(fovyRadians / 180.0f * M_PI, aspect, nearZ, farZ);
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedUp = ELQuaternionRotateVector3(cameraQuaternion, up);
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forward);

    ELVector3 transformedEye = ELVector3Add(eye, translation);
    ELVector3 transformedLookAt = ELVector3Add(transformedEye, transformedForward);

    ELMatrix4 lookAt = ELMatrix4MakeLookAt(transformedEye.x,transformedEye.y,transformedEye.z,transformedLookAt.x, transformedLookAt.y, transformedLookAt.z, transformedUp.x, transformedUp.y, transformedUp.z);
    return ELMatrix4Multiply(perspective, lookAt);
}

void ELCamera::rotateEye(ELFloat radians, ELVector3 axis) {
    ELQuaternion quaternion = ELQuaternionMakeWithAngleAndVector3Axis(radians, axis);
    up = ELQuaternionRotateVector3(quaternion,up);
    ELVector3 eyeVec = ELVector3Subtract(eye, lookAt);
    ELVector3 newEyeVec = ELQuaternionRotateVector3(quaternion, eyeVec);
    eye = ELVector3Add(lookAt, newEyeVec);
}

void ELCamera::rotateLookAtAroundUp(ELFloat radians) {
    radiansAroundUp += radians;
}

void ELCamera::rotateLookAtAroundLeft(ELFloat radians) {
    radiansAroundLeft += radians;
}

void ELCamera::rotateLookAtAroundForward(ELFloat radians) {
    radiansAroundForward += radians;
}

void ELCamera::translateForward(ELFloat distance) {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forwardVector());
    translation = ELVector3Add(translation, ELVector3Make(transformedForward.x * distance, transformedForward.y * distance, transformedForward.z * distance));
}

void ELCamera::translateUp(ELFloat distance) {
    translation = ELVector3Add(translation, ELVector3Make(up.x * distance, up.y * distance, up.z * distance));
}

void ELCamera::translateLeft(ELFloat distance) {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedLeft = ELQuaternionRotateVector3(cameraQuaternion, leftVector());
    translation = ELVector3Add(translation, ELVector3Make(transformedLeft.x * distance, transformedLeft.y * distance, transformedLeft.z * distance));
}

void ELCamera::translateTo(ELVector3 loc) {
    ELVector3 eyeVec = ELVector3Subtract(lookAt, eye);
    eye = loc;
    lookAt = ELVector3Add(eye, eyeVec);
}

void ELCamera::lockOn(ELTransform *transform) {
    lockOnTransform = transform;
}

void ELCamera::update(ELFloat timeInSecs) {
    if (lockOnTransform != NULL) {
        ELVector3 eyeVec = ELVector3Subtract(lookAt, eye);
        lookAt = lockOnTransform->position;
        eye = ELVector3Subtract(lookAt, eyeVec);
    }
}

// Private Methods
ELVector3 ELCamera::leftVector() {
    ELQuaternion quaternion = ELQuaternionMakeWithAngleAndVector3Axis(M_PI / 2.0, forwardVector());
    ELVector3 left = ELQuaternionRotateVector3(quaternion, up);
    return left;
}

ELVector3 ELCamera::forwardVector() {
    ELVector3 forward = ELVector3Normalize(ELVector3Subtract(lookAt, eye));
    return forward;
}

ELQuaternion ELCamera::quaternion() {
    ELVector3 forward = forwardVector();
    ELVector3 left = leftVector();
    ELQuaternion forwardQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundForward, forward.x, forward.y, forward.z);
    ELQuaternion upQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundUp / 180.0 * M_PI, up.x, up.y, up.z);
    ELQuaternion leftQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundLeft / 180.0 * M_PI, left.x, left.y, left.z);
    return ELQuaternionMultiply(ELQuaternionMultiply(forwardQuaternion, upQuaternion),leftQuaternion);
}