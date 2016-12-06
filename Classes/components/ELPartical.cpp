//
// Created by wangyang on 16/12/6.
//

#include "ELPartical.h"

#include <types/ELGeometryTypes.h>

void ELPartical::update(ELVector3 cameraPosition,ELFloat timeInSecs,ELVector3 force) {
    velocity = ELVector3Add(velocity, ELVector3MultiplyScalar(force, timeInSecs));
    ELTransform newTransform;
    newTransform.position = ELVector3Add(transform.position, ELVector3MultiplyScalar(velocity, timeInSecs));

    ELFloat cx = cameraPosition.x - transform.position.x;
    ELFloat cy = cameraPosition.y - transform.position.y;
    ELFloat cz = cameraPosition.z - transform.position.z;

    ELVector3 originNormal = {0,1,0};
    ELVector3 currentNormal = {cx,cy,cz};
    currentNormal = ELVector3Normalize(currentNormal);

    ELVector3 axis = ELVector3CrossProduct(originNormal,currentNormal);
    ELFloat half = ELVector3DotProduct(originNormal,currentNormal);
    newTransform.quaternion = ELQuaternionMake(axis.x, axis.y, axis.z , half);

    //caculate quad points
    ELGeometryRect rect = {
            {0.5f * size.x , 0.0f ,  -0.5f * size.y  },
            {0.5f * size.x , 0.0f ,  0.5f * size.y   },
            {-0.5f * size.x, 0.0f , 0.5f * size.y   },
            {-0.5f * size.x, 0.0f , -0.5f * size.y  },
            {0, 0},
            {0, 1},
            {1, 1},
            {1, 0}
    };

    ELMatrix4 modelMatrix = ELMatrix4MakeWithQuaternion(newTransform.quaternion);
    modelMatrix = ELMatrix4Multiply(modelMatrix, ELMatrix4MakeTranslation(newTransform.position.x,newTransform.position.y,newTransform.position.z));

    rect.point1 = ELMatrix4MultiplyVector3(modelMatrix, rect.point1);
    rect.point2 = ELMatrix4MultiplyVector3(modelMatrix, rect.point2);
    rect.point3 = ELMatrix4MultiplyVector3(modelMatrix, rect.point3);
    rect.point4 = ELMatrix4MultiplyVector3(modelMatrix, rect.point4);

    quadRect = rect;
}