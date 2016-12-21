//
// Created by wangyang on 16/11/24.
//

#include "ELCamera.h"

ELCamera::ELCamera()
{

}

ELCamera::ELCamera(ELVector3 eye,ELVector3 lookAt,ELVector3 up,ELFloat fovyRadians,ELFloat aspect,ELFloat nearZ,ELFloat farZ) :
        originEye(eye),
        originLookAt(lookAt),
        originUp(up),
        fovyRadians(fovyRadians),
        aspect(aspect),
        nearZ(nearZ),
        farZ(farZ),
        isOrtho(false),
        lockOnTransform(NULL),
        translation(ELVector3Make(0, 0, 0))
{
    radiansAroundForward = 0.0;
    radiansAroundUp = 0.0;
    radiansAroundLeft = 0.0;
    radiansAroundForwardLimit = ELVector2Make(0,2 * 180);
    radiansAroundUpLimit =  ELVector2Make(-2 * 180,2 * 180);
    radiansAroundLeftLimit =  ELVector2Make(0,2 * 180);
}

ELCamera::ELCamera(ELFloat left, ELFloat right, ELFloat top, ELFloat bottom, ELFloat nearZ, ELFloat farZ) :
        lockOnTransform(NULL),
        translation(ELVector3Make(0, 0, 0))
{
    radiansAroundForward = 0.0;
    radiansAroundUp = 0.0;
    radiansAroundLeft = 0.0;
    ortho(left, right, top, bottom, nearZ, farZ);
}

void ELCamera::ortho(ELFloat left, ELFloat right, ELFloat top, ELFloat bottom, ELFloat nearZ, ELFloat farZ) {
    isOrtho = true;
    orthoView = ELVector4Make(left,right,top,bottom);
    this->nearZ = nearZ;
    this->farZ = farZ;
}

ELMatrix4 ELCamera::matrix() {
    ELVector3 up = upVector();
    ELMatrix4 projection = ELMatrix4MakePerspective(fovyRadians / 180.0f * M_PI, aspect, nearZ, farZ);
    if (isOrtho) {
        projection = ELMatrix4MakeOrtho(orthoView.x,orthoView.y,orthoView.w,orthoView.z,nearZ,farZ);
    }
    ELVector3 eyePosition = position();
    ELVector3 desPosition = lookAtPosition();

    ELMatrix4 lookAt = ELMatrix4MakeLookAt(eyePosition.x,eyePosition.y,eyePosition.z,desPosition.x, desPosition.y, desPosition.z, up.x, up.y, up.z);
    ELMatrix4 finalMatrix = ELMatrix4Multiply(projection, lookAt);
    return finalMatrix;
}

void ELCamera::rotateEye(ELFloat radians, ELVector3 axis) {
    ELQuaternion quaternion = ELQuaternionMakeWithAngleAndVector3Axis(radians, axis);
    originUp = ELQuaternionRotateVector3(quaternion, originUp);
    ELVector3 eyeVec = ELVector3Subtract(originEye, originLookAt);
    ELVector3 newEyeVec = ELQuaternionRotateVector3(quaternion, eyeVec);
    originEye = ELVector3Add(originLookAt, newEyeVec);
}

void ELCamera::rotateLookAtAroundUp(ELFloat radians) {
    radiansAroundUp += radians;
    while (fabs(radiansAroundUp) > 2 * 180) {
        radiansAroundUp = -radiansAroundUp + 2 * 180 * fabs(radiansAroundUp) / radiansAroundUp;
    }

    if (radiansAroundUp > radiansAroundUpLimit.y) {
        radiansAroundUp = radiansAroundUpLimit.y;
    }
    if (radiansAroundUp < radiansAroundUpLimit.x) {
        radiansAroundUp = radiansAroundUpLimit.x;
    }
}

void ELCamera::rotateLookAtAroundLeft(ELFloat radians) {
    radiansAroundLeft += radians;
    while (fabs(radiansAroundLeft) > 2 * 180) {
        radiansAroundLeft = -radiansAroundLeft + 2 * 180 * fabs(radiansAroundLeft) / radiansAroundLeft;
    }
    if (radiansAroundLeft > radiansAroundLeftLimit.y) {
        radiansAroundLeft = radiansAroundLeftLimit.y;
    }
    if (radiansAroundLeft < radiansAroundLeftLimit.x) {
        radiansAroundLeft = radiansAroundLeftLimit.x;
    }
}

void ELCamera::rotateLookAtAroundForward(ELFloat radians) {
    radiansAroundForward += radians;
    if (radiansAroundForward > radiansAroundForwardLimit.y) {
        radiansAroundForward = radiansAroundForwardLimit.y;
    }
    if (radiansAroundForward < radiansAroundForwardLimit.x) {
        radiansAroundForward = radiansAroundForwardLimit.x;
    }
}

void ELCamera::translateForward(ELFloat distance) {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forwardVector());
    translation = ELVector3Add(translation, ELVector3Make(transformedForward.x * distance, transformedForward.y * distance, transformedForward.z * distance));
}

void ELCamera::translateUp(ELFloat distance) {
    translation = ELVector3Add(translation, ELVector3Make(
            originUp.x * distance, originUp.y * distance, originUp.z * distance));
}

void ELCamera::translateLeft(ELFloat distance) {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedLeft = ELQuaternionRotateVector3(cameraQuaternion, leftVector());
    translation = ELVector3Add(translation, ELVector3Make(transformedLeft.x * distance, transformedLeft.y * distance, transformedLeft.z * distance));
}

void ELCamera::translateTo(ELVector3 loc) {
    ELVector3 eyeVec = ELVector3Subtract(originLookAt, originEye);
    originEye = loc;
    originLookAt = ELVector3Add(originEye, eyeVec);
}

void ELCamera::lockOn(ELTransform *transform) {
    lockOnTransform = transform;
}

void ELCamera::update(ELFloat timeInSecs) {
    if (lockOnTransform != NULL) {
//        ELVector3 eyeVec = ELVector3Subtract(originLookAt, originEye);
//        originLookAt = lockOnTransform->position;
//        originEye = ELVector3Subtract(originLookAt, eyeVec);

        ELVector3 eyeVec = ELVector3Subtract(originLookAt, originEye);
        originEye = lockOnTransform->position;
        originEye.y += 2;
        originLookAt = ELVector3Add(originEye, eyeVec);
    }
}

ELVector3 ELCamera::leftVector() {
    float rotateAngle = M_PI / 2.0;
    ELQuaternion quaternion = ELQuaternionMakeWithAngleAndVector3Axis(rotateAngle, forwardVector());
    ELVector3 left = ELQuaternionRotateVector3(quaternion, originUp);
    return ELVector3Normalize(left);
}

ELVector3 ELCamera::forwardVector() {
    ELVector3 forward = ELVector3Normalize(ELVector3Subtract(originLookAt, originEye));
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedForward = ELQuaternionRotateVector3(cameraQuaternion, forward);
    return ELVector3Normalize(transformedForward);
}

ELVector3 ELCamera::upVector() {
    ELQuaternion cameraQuaternion = quaternion();
    ELVector3 transformedUp = ELQuaternionRotateVector3(cameraQuaternion, originUp);
    return ELVector3Normalize(transformedUp);
}

ELVector3 ELCamera::position() {
    ELVector3 transformedEye = ELVector3Add(originEye, translation);
    return transformedEye;
}

ELVector3 ELCamera::lookAtPosition() {
    ELVector3 transformedForward = forwardVector();
    ELVector3 transformedEye = position();
    ELVector3 transformedLookAt = ELVector3Add(transformedEye, transformedForward);
    return transformedLookAt;
}

ELQuaternion ELCamera::quaternion() {
    float rotateAngle = M_PI / 2.0;
    ELVector3 forward = ELVector3Normalize(ELVector3Subtract(originLookAt, originEye));
    ELVector3 left = ELQuaternionRotateVector3(ELQuaternionMakeWithAngleAndVector3Axis(rotateAngle, forward), originUp);
    left = ELVector3Normalize(left);
    ELQuaternion forwardQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundForward, forward.x, forward.y, forward.z);
    ELQuaternion upQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundUp / 180.0 * M_PI, originUp.x, originUp.y, originUp.z);
    ELQuaternion leftQuaternion = ELQuaternionMakeWithAngleAndAxis(radiansAroundLeft / 180.0 * M_PI, left.x, left.y, left.z);
    return ELQuaternionMultiply(ELQuaternionMultiply(forwardQuaternion, upQuaternion),leftQuaternion);
}