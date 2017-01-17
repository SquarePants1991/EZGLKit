//
// Created by wangyang on 17/1/12.
//

#ifndef EZGL_ELFBXVERTEXBUFFERPROVIDER_H
#define EZGL_ELFBXVERTEXBUFFERPROVIDER_H

#include "component/geometry/ELVertexBufferGeometry.h"
#include "fbxsdk.h"

class ELFBXVertexBufferProvider : public ELGeometryVertexBufferProvider {
public:
    ELFBXVertexBufferProvider(FbxScene* scene);
    void prepare();
    std::map<std::string, ELAnimation> loadAnimations();
    void update(const char *animationName,ELFloat elapsedTime, ELGeometryVertexBuffer *vertexBuffer);
    virtual void update(ELFloat timeInSecs, ELGeometryVertexBuffer *vertexBuffer);
private:
    FbxScene *scene;
    FbxMesh *mesh; // process only one mesh now
    FbxPose *currentPose;

private:
    ~ELFBXVertexBufferProvider();
    void loadNodes();
    void computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime time, FbxVector4 *vertices);
    void computerClusterDeformation(FbxAMatrix& pGlobalPosition,FbxTime &pTime, FbxPose *pPose, FbxMesh *pMesh, FbxCluster *pCluster, FbxAMatrix& pVertexTransformMatrix);
    void loadVerticesToVertexBuffer(FbxMesh *mesh, FbxVector4 *pVertices, ELGeometryVertexBuffer *vertexBuffer);
};


#endif //EZGL_ELFBXVERTEXBUFFERPROVIDER_H
