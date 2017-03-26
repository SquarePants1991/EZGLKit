//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELWAVEFRONTLOADER_H
#define EZGL_ELWAVEFRONTLOADER_H

#include "../component/ELGeometry.h"
#include "ELGeometryVertexBuffer.h"

#include <vector>
#include <string>

class ELMeshGeometry;

class ELWaveFrontLoader {
public:
    static std::vector<ELMeshGeometry *> loadFile(std::string filePath);
};


#endif //EZGL_ELWAVEFRONTLOADER_H
