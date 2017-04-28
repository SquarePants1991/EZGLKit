//
// Created by wangyang on 16/12/30.
//

#ifndef EZGL_ELDAELOADER_H
#define EZGL_ELDAELOADER_H


#include "../component/geometry/ELMeshGeometry.h"
#include <vector>

class ELMeshGeometry;
class ELGeometryVertexBuffer;

namespace fbxsdk_2015_1 {
    class FbxMesh;

    class FbxScene;

    class FbxAMatrix;

    class FbxTime;

    class FbxPose;

    class FbxCluster;

    class FbxManager;
}

using namespace fbxsdk_2015_1;

class ELFBXLoader {
public:
    static void setupFbxEnv();
    static std::vector<ELMeshGeometry *> loadFromFile(const char *filePath);
    static ELMeshGeometry *loadMesh(FbxMesh *mesh);
    static std::map<std::string, ELAnimation> loadAnimationNames(FbxScene *scene);
    static void loadAnimation(FbxScene *scene);
    static void loadSkin(FbxMesh *mesh);
    static void computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime& time);
    static void computerClusterDeformation(FbxAMatrix& pGlobalPosition,FbxTime &pTime, FbxPose *pPose, FbxMesh *mesh, FbxCluster *cluster, FbxAMatrix& vertexTransform);
private:
    static FbxManager *fbxManager;
    static void loadMaterial(ELMeshGeometry *geometry, FbxMesh *mesh);
    static void generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData);
};


#endif //EZGL_ELDAELOADER_H
