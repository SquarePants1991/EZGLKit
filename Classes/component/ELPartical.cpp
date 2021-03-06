//
// Created by wangyang on 16/12/6.
//

#include "ELPartical.h"

#include "../types/ELGeometryTypes.h"

ELPartical::ELPartical() : currentAge(0) {

}

void ELPartical::update(ELVector3 cameraPosition,ELFloat timeInSecs,ELVector3 force) {
    velocity = ELVector3Add(velocity, ELVector3MultiplyScalar(force, timeInSecs));
    ELTransform newTransform;
    newTransform.position = ELVector3Add(transform.position, ELVector3MultiplyScalar(velocity, timeInSecs));

    ELFloat cx = cameraPosition.x - transform.position.x;
    ELFloat cy = cameraPosition.y - transform.position.y;
    ELFloat cz = cameraPosition.z - transform.position.z;

    ELVector3 originNormal = {0,0,1};
    ELVector3 currentNormal = {cx,cy,cz};
    currentNormal = ELVector3Normalize(currentNormal);

    ELVector3 halfVec = ELVector3Normalize(ELVector3Add(originNormal,currentNormal));
    ELFloat half = ELVector3DotProduct(originNormal,halfVec);
    ELVector3 axis = ELVector3CrossProduct(originNormal,halfVec);

    newTransform.quaternion = ELQuaternionMake(axis.x , axis.y, axis.z, half);

    ELFloat angle = 0;
    if (cz == 0) {
        angle = 0;
    } else {
        ELFloat tanVal = cx / cz;
        angle = atan(tanVal);
    }
    newTransform.quaternion = ELQuaternionMakeWithAngleAndAxis(angle,0,1,0);



    size.x = sizeStart.x + (sizeEnd.x - sizeStart.x) / age * currentAge;
    size.y = sizeStart.y + (sizeEnd.y - sizeStart.y) / age * currentAge;

    color.r = colorStart.x + (colorEnd.x - colorStart.x) / age * currentAge;
    color.g = colorStart.y + (colorEnd.y - colorStart.y) / age * currentAge;
    color.b = colorStart.z + (colorEnd.z - colorStart.z) / age * currentAge;
    color.a = colorStart.w + (colorEnd.w - colorStart.w) / age * currentAge;

    rotation = rotationStart + (rotationStart - rotationEnd) / age * currentAge;

    //caculate quad points
    ELGeometryColorRect rect = {
            {
                {0.5f * size.x  ,  0.5f * size.y  , 0.0f  },
                {0.5f * size.x  ,  -0.5f * size.y , 0.0f  },
                {-0.5f * size.x , -0.5f * size.y  , 0.0f },
                {-0.5f * size.x , 0.5f * size.y   , 0.0f },
                {1, 0},
                {1, 1},
                {0, 1},
                {0, 0},
            },
            {1.0, 1.0, 0.0, 1.0},
            {1.0, 1.0, 0.0, 1.0},
            {1.0, 1.0, 0.0, 1.0},
            {1.0, 1.0, 0.0, 1.0}
    };

    ELQuaternion rotateQuaternion = ELQuaternionMakeWithAngleAndAxis(rotation / 180.0 * M_PI, 0,0,1);
    ELMatrix4 rotateMatrix = ELMatrix4MakeWithQuaternion(rotateQuaternion);
    rect.geometryRect.point1 = ELMatrix4MultiplyVector4(rotateMatrix, ELVector4MakeWithVector3(rect.geometryRect.point1,1)).xyz;
    rect.geometryRect.point2 = ELMatrix4MultiplyVector4(rotateMatrix, ELVector4MakeWithVector3(rect.geometryRect.point2,1)).xyz;
    rect.geometryRect.point3 = ELMatrix4MultiplyVector4(rotateMatrix, ELVector4MakeWithVector3(rect.geometryRect.point3,1)).xyz;
    rect.geometryRect.point4 = ELMatrix4MultiplyVector4(rotateMatrix, ELVector4MakeWithVector3(rect.geometryRect.point4,1)).xyz;

    ELMatrix4 modelMatrix;
    ELMatrix4 translateMatrix = ELMatrix4MakeTranslation(newTransform.position.x,newTransform.position.y,newTransform.position.z);
    ELMatrix4 quaternionMatrix = ELMatrix4MakeWithQuaternion(newTransform.quaternion);
    modelMatrix = ELMatrix4Multiply(translateMatrix, quaternionMatrix);

    rect.geometryRect.point1 = ELMatrix4MultiplyVector4(modelMatrix, ELVector4MakeWithVector3(rect.geometryRect.point1,1)).xyz;
    rect.geometryRect.point2 = ELMatrix4MultiplyVector4(modelMatrix, ELVector4MakeWithVector3(rect.geometryRect.point2,1)).xyz;
    rect.geometryRect.point3 = ELMatrix4MultiplyVector4(modelMatrix, ELVector4MakeWithVector3(rect.geometryRect.point3,1)).xyz;
    rect.geometryRect.point4 = ELMatrix4MultiplyVector4(modelMatrix, ELVector4MakeWithVector3(rect.geometryRect.point4,1)).xyz;

    rect.color1 = color;
    rect.color2 = color;
    rect.color3 = color;
    rect.color4 = color;

    transform = newTransform;
    quadRect = rect;

    currentAge += timeInSecs;
}

bool ELPartical::isAlive() {
    return age > currentAge;
}

void ELPartical::reset() {
    currentAge = 0;
}