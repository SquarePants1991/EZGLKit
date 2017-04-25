//
// Created by wangyang on 17/1/12.
//

#ifndef EZGL_ELFBXVERTEXBUFFERPROVIDER_H
#define EZGL_ELFBXVERTEXBUFFERPROVIDER_H

#include "../component/geometry/ELVertexBufferGeometry.h"

namespace fbxsdk_2015_1 {
    class FbxMesh;

    class FbxScene;

    class FbxAMatrix;

    class FbxTime;

    class FbxPose;

    class FbxCluster;

    class FbxNode;

    class FbxAnimLayer;

    class FbxVector4;
};

using namespace fbxsdk_2015_1;

class ELFBXVertexBufferProvider : public ELGeometryVertexBufferProvider {
public:
    ELFBXVertexBufferProvider(FbxScene* scene, FbxMesh *mesh);
    void prepare();
    std::map<std::string, ELAnimation> loadAnimations();
    virtual void update(const char *animationName,ELFloat elapsedTime, ELGeometryVertexBuffer *vertexBuffer);
    virtual std::string kind() { return "buffer provider"; };
private:
    FbxScene *scene;
    FbxMesh *mesh; // process only one mesh now
    FbxPose *currentPose;

private:
    ~ELFBXVertexBufferProvider();
    std::vector<ELVector2> uvCache;
    long long uvCacheLen;
    long long uvCacheAvaliableIndex;
    void loadNodes();
    void computeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, FbxVector4* pVertexArray);
    void computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime& time, FbxVector4 *vertices);
    void computerClusterDeformation(FbxAMatrix& pGlobalPosition,FbxTime &pTime, FbxPose *pPose, FbxMesh *pMesh, FbxCluster *pCluster, FbxAMatrix& pVertexTransformMatrix);
    void loadVerticesToVertexBuffer(FbxMesh *mesh, FbxVector4 *pVertices, ELGeometryVertexBuffer *vertexBuffer);
    void addUVToCache(int polyIndex,int pointIndex, ELVector2 uv);
    ELVector2 getUVFromCache(int polyIndex,int pointIndex, bool &success);
    void FbxLoadTrianglePoint(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELVector3 &pPos,ELVector2 &pUV);
};


#endif //EZGL_ELFBXVERTEXBUFFERPROVIDER_H
