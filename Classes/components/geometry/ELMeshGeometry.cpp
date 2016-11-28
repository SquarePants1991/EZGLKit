//
// Created by wangyang on 16/11/28.
//

#include "ELMeshGeometry.h"

ELMeshGeometry::ELMeshGeometry(ELGeometryData data) : _data(data) {

}

ELGeometryData ELMeshGeometry::generateData() {
    return _data;
}