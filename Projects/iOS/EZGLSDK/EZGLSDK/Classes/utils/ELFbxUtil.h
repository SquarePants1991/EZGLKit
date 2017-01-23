//
// Created by wangyang on 17/1/12.
//

#ifndef EZGL_ELFBXMATRIXUTIL_H
#define EZGL_ELFBXMATRIXUTIL_H
#include <fbxsdk.h>
#include "types/EZGLTypes.h"

extern void FbxAMatrixScale(FbxAMatrix& pMatrix, double scalar);
extern void FbxAMatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);
extern FbxAMatrix FbxGetPoseMatrix(FbxPose* pPose, int pNodeIndex);
extern FbxAMatrix FbxGetNodeGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition = NULL);
extern void FbxLoadTrianglePoint(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELVector3 &pPos,ELVector2 &pUV);
extern FbxAMatrix FbxGetNodeGeometricTransform(FbxNode *pNode);
#endif //EZGL_ELFBXMATRIXUTIL_H
