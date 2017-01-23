//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELCAMERA_H
#define EZGL_ELCAMERA_H

#include "types/EZGLTypes.h"
#include "ELNode.h"

class ELCamera : public ELNode {
public:
    ELVector3 originEye;
    ELVector3 originLookAt;
    ELVector3 originUp;
    ELFloat nearZ;
    ELFloat farZ;
    ELFloat fovyRadians;
    ELFloat aspect;

    ELFloat radiansAroundLeft;
    ELVector2 radiansAroundLeftLimit;
    ELFloat radiansAroundUp;
    ELVector2 radiansAroundUpLimit;
    ELFloat radiansAroundForward;
    ELVector2 radiansAroundForwardLimit;

    ELVector3 translation;

    ELVector4 orthoView;

public:
    ELCamera();
    // Perspective Camera
    ELCamera(ELVector3 eye,ELVector3 lookAt,ELVector3 up,ELFloat fovyRadians,ELFloat aspect,ELFloat nearZ,ELFloat farZ);
    void perspective(ELVector3 eye,ELVector3 lookAt,ELVector3 up,ELFloat fovyRadians,ELFloat aspect,ELFloat nearZ,ELFloat farZ);
    // Perspective Camera
    ELCamera(ELFloat left, ELFloat right, ELFloat top, ELFloat bottom, ELFloat nearZ, ELFloat farZ);
    void ortho(ELFloat left, ELFloat right, ELFloat top, ELFloat bottom, ELFloat nearZ, ELFloat farZ);

    ELMatrix4 matrix();
    void rotateEye(ELFloat radians, ELVector3 axis);
    void rotateLookAtAroundUp(ELFloat radians);
    void rotateLookAtAroundLeft(ELFloat radians);
    void rotateLookAtAroundForward(ELFloat radians);
    void translateForward(ELFloat distance);
    void translateUp(ELFloat distance);
    void translateLeft(ELFloat distance);
    void translateTo(ELVector3 loc);

    void lockOn(ELTransform *transform);

    ELVector3 leftVector();
    ELVector3 forwardVector();
    ELVector3 upVector();

    ELVector3 position();
    ELVector3 lookAtPosition();

    virtual void update(ELFloat timeInSecs);

private:
    bool isOrtho;
    bool needFlip;

    ELQuaternion quaternion();

    ELTransform * lockOnTransform;
};


#endif //EZGL_ELCAMERA_H
