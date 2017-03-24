//
// Created by wangyang on 17/1/12.
//

#ifndef EZGL_ELFBXMATRIXUTIL_H
#define EZGL_ELFBXMATRIXUTIL_H

#include "../types/EZGLTypes.h"

namespace fbxsdk_2015_1 {
    class FbxMesh;

    class FbxScene;

    class FbxAMatrix;

    class FbxTime;

    class FbxPose;

    class FbxCluster;

    class FbxNode;
}

extern void FbxAMatrixScale(fbxsdk_2015_1::FbxAMatrix& pMatrix, double scalar);
extern void FbxAMatrixAdd(fbxsdk_2015_1::FbxAMatrix& pDstMatrix, fbxsdk_2015_1::FbxAMatrix& pSrcMatrix);
extern fbxsdk_2015_1::FbxAMatrix FbxGetPoseMatrix(fbxsdk_2015_1::FbxPose* pPose, int pNodeIndex);
extern fbxsdk_2015_1::FbxAMatrix FbxGetNodeGlobalPosition(fbxsdk_2015_1::FbxNode* pNode, const fbxsdk_2015_1::FbxTime& pTime, fbxsdk_2015_1::FbxPose* pPose, fbxsdk_2015_1::FbxAMatrix* pParentGlobalPosition = 0);
extern fbxsdk_2015_1::FbxAMatrix FbxGetNodeGeometricTransform(fbxsdk_2015_1::FbxNode *pNode);
#endif //EZGL_ELFBXMATRIXUTIL_H
