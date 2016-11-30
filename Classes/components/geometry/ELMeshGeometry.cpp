//
// Created by wangyang on 16/11/28.
//

#include <core/ELGameObject.h>
#include "ELMeshGeometry.h"

ELMeshGeometry::ELMeshGeometry(ELGeometryData data) : _data(data) {

}

ELGeometryData ELMeshGeometry::generateData() {
    return _data;
}

void ELMeshGeometry::update(ELFloat timeInSecs) {
    ELGeometry::update(timeInSecs);
//    static float angle = 0.0;
//
//    ELGameObject *gameObj = (ELGameObject *)gameObject();
//    gameObj->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(angle,1,1,1);
//    angle += timeInSecs;
////    printf("angle => %f \n", timeInSecs);
}