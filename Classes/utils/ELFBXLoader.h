//
// Created by wangyang on 16/12/30.
//

#ifndef EZGL_ELDAELOADER_H
#define EZGL_ELDAELOADER_H

#include <fbxsdk.h>
#include "component/ELGeometry.h"
#include <vector>

class ELGeometryVertexBuffer;
class ELMeshGeometry;

class ELFBXLoader {
public:
    static void setupFbxEnv();
    static std::vector<ELMeshGeometry *> loadFromFile(const char *filePath);
    static ELMeshGeometry *loadMesh(FbxMesh *mesh);

private:
    static FbxManager *fbxManager;

    static void generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData);
};


#endif //EZGL_ELDAELOADER_H
