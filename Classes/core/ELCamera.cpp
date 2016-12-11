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
    isOrtho = false;
    translation = ELVector3Make(0, 0, 0);
    radiansAroundForward = 0.0;
    radiansAroundUp = 0.0;
    radiansAroundLeft = 0.0;
}

void ELCamera::asOrtho(ELFloat left,ELFloat right,ELFloat top,ELFloat bottom,ELFloat nearZ,ELFloat farZ) {
    isOrtho = true;
    orthoView = ELVector4Make(left,right,top,bottom);
    this->nearZ = nearZ;
    this->farZ = farZ;
}

ELMatrix4 ELCamera::matrix() {
    ELVector3 _up = up;
    if (needFlip) {
        _up = ELMatrix4MultiplyVector4(ELMatrix4MakeScale(1,-1,1), ELVector4Make(_up.x,_up.y,_up.z,1)).xyz;
    }
    ELVector3 forward = forwardVector();
    ELVector3 left = leftVector();
    ELMatrix4 projection = ELMatrix4MakePerspective(fovyRadians / 180.0f * M_PI, aspect, nearZ, farZ);
    if (isOrtho) {
        projection = ELMatrix4MakeOrtho(orthoView.x,orthoView.y,orthoView.w,orthoView.z,nearZ,farZ);
    }
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedUp = ELQuaternionRotateVector3(cameraQuaternion, _up);
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forward);

    ELVector3 transformedEye = ELVector3Add(eye, translation);
    if (needFlip) {
        transformedEye = ELMatrix4MultiplyVector4(ELMatrix4MakeScale(1,-1,1), ELVector4Make(transformedEye.x,transformedEye.y,transformedEye.z,1)).xyz;
    }
    ELVector3 transformedLookAt = ELVector3Add(transformedEye, transformedForward);

    ELMatrix4 lookAt = ELMatrix4MakeLookAt(transformedEye.x,transformedEye.y,transformedEye.z,transformedLookAt.x, transformedLookAt.y, transformedLookAt.z, transformedUp.x, transformedUp.y, transformedUp.z);
    ELMatrix4 finalMatrix = ELMatrix4Multiply(projection, lookAt);
    if (needFlip) {
       // finalMatrix = ELMatrix4Multiply(ELMatrix4MakeScale(1,-1,1),finalMatrix);
    }
    return finalMatrix;
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
//        ELVector3 eyeVec = ELVector3Subtract(lookAt, eye);
//        lookAt = lockOnTransform->position;
//        eye = ELVector3Subtract(lookAt, eyeVec);

        ELVector3 eyeVec = ELVector3Subtract(lookAt, eye);
        eye = lockOnTransform->position;
        eye.y += 2;
        lookAt = ELVector3Add(eye, eyeVec);
    }
}

// caculated direction
ELVector3 ELCamera::forward() {
    ELVector3 forward = forwardVector();
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forward);
    return transformedForward;
}

ELVector3 ELCamera::position() {
    ELVector3 transformedEye = ELVector3Add(eye, translation);
    return transformedEye;
}

ELVector3 ELCamera::lookAtPosition() {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forward());

    ELVector3 transformedEye = ELVector3Add(eye, translation);
    ELVector3 transformedLookAt = ELVector3Add(transformedEye, transformedForward);
    return transformedLookAt;
}

void ELCamera::flip(bool flip) {
    needFlip = flip;
}

// Private Methods
ELVector3 ELCamera::leftVector() {
    float rotateAngle = M_PI / 2.0;
    if (needFlip) {
        rotateAngle = -M_PI / 2.0;
    }
    ELQuaternion quaternion = ELQuaternionMakeWithAngleAndVector3Axis(rotateAngle, forwardVector());
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