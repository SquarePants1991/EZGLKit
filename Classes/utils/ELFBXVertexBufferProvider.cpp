//
// Created by wangyang on 17/1/12.
//

#include "ELFBXVertexBufferProvider.h"
#include "ELFbxUtil.h"
#include <fbxsdk.h>

ELFBXVertexBufferProvider::ELFBXVertexBufferProvider(FbxScene *scene, FbxMesh *mesh) :
        scene(scene),
        mesh(mesh),
        uvCacheAvaliableIndex(-1)
{
}

ELFBXVertexBufferProvider::~ELFBXVertexBufferProvider() {
}

void ELFBXVertexBufferProvider::prepare() {
    loadNodes();
    collectClusters();
    collectVertex();
}

void ELFBXVertexBufferProvider::loadNodes() {
    int poseCount = scene->GetPoseCount();
    currentPose = scene->GetPose(0);
}

void ELFBXVertexBufferProvider::collectClusters() {
    int clusterCount = 0;
    int vertexCount = mesh->GetControlPointsCount();
    vertexClusters.clear();
    
    for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); ++i) {
        FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(i, FbxDeformer::eSkin);
        clusterCount = skin->GetClusterCount();
        for (int i = 0;i < clusterCount; ++i) {
            FbxCluster *cluster = skin->GetCluster(i);
            clusters.push_back(cluster);
            
            // 收集每个顶点的Clusters
            int lVertexIndexCount = cluster->GetControlPointIndicesCount();
            for (int k = 0; k < lVertexIndexCount; ++k) {
                int lIndex = cluster->GetControlPointIndices()[k];
                if (lIndex < vertexCount) {
                    double lWeight = cluster->GetControlPointWeights()[k];
                    vertexClusters[lIndex].push_back(std::make_pair(clusters.size() - 1, (float)lWeight));
                }
            }
            
        }
    }
    
    for (int i = 0; i < clusters.size(); ++i) {
        clustersMatrix.push_back(ELMatrix4Identity);
    }
}

void ELFBXVertexBufferProvider::collectVertex() {
    vertexBufferCache = retain_ptr_init(ELGeometryVertexBuffer);
    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4 *vertexArray = new FbxVector4[vertexCount];
    memcpy(vertexArray, mesh->GetControlPoints(), vertexCount * sizeof(FbxVector4));
    loadVerticesToVertexBuffer(mesh, vertexArray, vertexBufferCache.get());
}

void ELFBXVertexBufferProvider::updateClusters(FbxTime time) {
    FbxAMatrix globalMatrix = FbxGetNodeGlobalPosition(mesh->GetNode(), time, currentPose, NULL);
    int index = 0;
    for (auto iter = clusters.begin(); iter != clusters.end(); ++iter) {
        FbxCluster *cluster = *iter;
        FbxAMatrix vertexTransform;
        computerClusterDeformation(globalMatrix, time, currentPose, mesh, cluster, vertexTransform);
        double* matrixData = (double*)vertexTransform;
        ELMatrix4 elMatrix;
        for (int i = 0; i < 16; ++i) {
            elMatrix.m[i] = (ELFloat)matrixData[i];
        }
        clustersMatrix[index++] = elMatrix;
    }
}

void ELFBXVertexBufferProvider::update(const char *animationName,ELFloat elapsedTime, ELGeometryVertexBuffer *vertexBuffer, std::vector<ELMatrix4> &clustersMatrix) {
    FbxTime fbxElapsedTime(FbxLongLong(FBXSDK_TC_SECOND*elapsedTime));
    updateClusters(fbxElapsedTime);
    clustersMatrix = this->clustersMatrix;
    vertexBuffer->setRawData(vertexBufferCache->data(), vertexBufferCache->rawLength());
}

void ELFBXVertexBufferProvider::computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime& time, FbxVector4 *vertexArray) {


    FbxCluster::ELinkMode clusterMode = ((FbxSkin *) mesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(
            0)->GetLinkMode();
    int vertexCount = mesh->GetControlPointsCount();

    FbxAMatrix *clusterDeformation = new FbxAMatrix[vertexCount];
    memset(clusterDeformation, 0, vertexCount * sizeof(FbxAMatrix));

    double *clusterWeight = new double[vertexCount];
    memset(clusterWeight, 0, sizeof(double) * vertexCount);


    if (clusterMode == FbxCluster::eAdditive) {
        for (int i = 0; i < vertexCount; ++i) {
            clusterDeformation[i].SetIdentity();
        }
    }

    int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int j = 0; j < skinCount; ++j) {
        FbxSkin *skin = static_cast<FbxSkin *>(mesh->GetDeformer(j, FbxDeformer::eSkin));
        int clusterCount = skin->GetClusterCount();
        for (int i = 0; i < clusterCount; ++i) {
            FbxCluster *cluster = skin->GetCluster(i);
            if (!cluster->GetLink()) continue;
            FbxAMatrix vertexTransformMatrix;
            computerClusterDeformation(pGlobalPosition, time, NULL, mesh, cluster, vertexTransformMatrix);
            int lVertexIndexCount = cluster->GetControlPointIndicesCount();
            for (int k = 0; k < lVertexIndexCount; ++k) {
                int lIndex = cluster->GetControlPointIndices()[k];

                // Sometimes, the mesh can have less points than at the time of the skinning
                // because a smooth operator was active when skinning but has been deactivated during export.
                if (lIndex >= vertexCount)
                    continue;

                double lWeight = cluster->GetControlPointWeights()[k];

                if (lWeight == 0.0) {
                    continue;
                }

                // Compute the influence of the link on the vertex.
                FbxAMatrix lInfluence = vertexTransformMatrix;
                if (lIndex == 1) {

                }
                FbxAMatrixScale(lInfluence, lWeight);

                if (clusterMode == FbxCluster::eAdditive) {
//                    // Multiply with the product of the deformations on the vertex.
//                    MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
                    clusterDeformation[lIndex] = lInfluence * clusterDeformation[lIndex];
//
//                    // Set the link to 1.0 just to know this vertex is influenced by a link.
                    clusterWeight[lIndex] = 1.0;
                }
                else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
                {
                    // Add to the sum of the deformations on the vertex.
                    FbxAMatrixAdd(clusterDeformation[lIndex], lInfluence);

                    // Add to the sum of weights to either normalize or complete the vertex.lif ()
                    if (lIndex == 1) {

                    }
                    clusterWeight[lIndex] += lWeight;
//                    if (lIndex == 1) {
//                        printf("Weight -- 1: %f : %f", lWeight, clusterWeight[lIndex]);
//                    }
                }
            }//For each vertex
        }
    }
    //Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
    for (int i = 0; i < vertexCount; i++) {
        FbxVector4 lSrcVertex = vertexArray[i];
        FbxVector4 &lDstVertex = vertexArray[i];
        double lWeight = clusterWeight[i];

        // Deform the vertex if there was at least a link with an influence on the vertex,
        if (lWeight != 0.0) {
            lDstVertex = clusterDeformation[i].MultT(lSrcVertex);
            if (clusterMode == FbxCluster::eNormalize) {
                // In the normalized link mode, a vertex is always totally influenced by the links.
                lDstVertex /= lWeight;
            }
            else if (clusterMode == FbxCluster::eTotalOne) {
                // In the total 1 link mode, a vertex can be partially influenced by the links.
                lSrcVertex *= (1.0 - lWeight);
                lDstVertex += lSrcVertex;
            }
        }
    }

    delete [] clusterWeight;
    delete [] clusterDeformation;
}

void ELFBXVertexBufferProvider::computerClusterDeformation(FbxAMatrix& pGlobalPosition,FbxTime &pTime, FbxPose *pPose, FbxMesh *pMesh, FbxCluster *pCluster, FbxAMatrix& pVertexTransformMatrix) {
    FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

    FbxAMatrix lReferenceGlobalInitPosition;
    FbxAMatrix lReferenceGlobalCurrentPosition;
    FbxAMatrix lAssociateGlobalInitPosition;
    FbxAMatrix lAssociateGlobalCurrentPosition;
    FbxAMatrix lClusterGlobalInitPosition;
    FbxAMatrix lClusterGlobalCurrentPosition;

    FbxAMatrix lReferenceGeometry;
    FbxAMatrix lAssociateGeometry;
    FbxAMatrix lClusterGeometry;

    FbxAMatrix lClusterRelativeInitPosition;
    FbxAMatrix lClusterRelativeCurrentPositionInverse;

    if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
    {
        pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
        // Model Matrix
        // Geometric transform of the model
        lAssociateGeometry = FbxGetNodeGeometricTransform(pCluster->GetAssociateModel());
        lAssociateGlobalInitPosition *= lAssociateGeometry;
        lAssociateGlobalCurrentPosition = FbxGetNodeGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

        pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
        // Mesh Matrix
        // Multiply lReferenceGlobalInitPosition by Geometric Transformation
        lReferenceGeometry = FbxGetNodeGeometricTransform(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;
        lReferenceGlobalCurrentPosition = pGlobalPosition;

        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        // Multiply lClusterGlobalInitPosition by Geometric Transformation
        lClusterGeometry = FbxGetNodeGeometricTransform(pCluster->GetLink());
        lClusterGlobalInitPosition *= lClusterGeometry;
        lClusterGlobalCurrentPosition = FbxGetNodeGlobalPosition(pCluster->GetLink(), pTime, pPose);

        // Compute the shift of the link relative to the reference.
        //ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
        pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
                                 lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
    }
    else
    {
        pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
        lReferenceGlobalCurrentPosition = pGlobalPosition;
        // Multiply lReferenceGlobalInitPosition by Geometric Transformation
        lReferenceGeometry = FbxGetNodeGeometricTransform(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;
        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        lClusterGlobalCurrentPosition = FbxGetNodeGlobalPosition(pCluster->GetLink(), pTime, pPose);
        
        // Compute the initial position of the link relative to the reference.
        lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

        // Compute the current position of the link relative to the reference.
        lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

        // Compute the shift of the link relative to the reference.
        pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
    }
}

void ELFBXVertexBufferProvider::loadVerticesToVertexBuffer(FbxMesh *mesh, FbxVector4 *pVertices, ELGeometryVertexBuffer *vertexBuffer) {
    int polyCount = mesh->GetPolygonCount();
    for (int i = 0; i < polyCount; ++i) {
        int polySize = mesh->GetPolygonSize(i);
        // load material
        FbxGeometryElementMaterial *materialElement = mesh->GetElementMaterial(0);
        FbxSurfaceMaterial *material = NULL;
        int matId = 0;
        if (materialElement->GetIndexArray().GetCount() > i) {
            material = mesh->GetNode()->GetMaterial(materialElement->GetIndexArray().GetAt(i));
            matId = materialElement->GetIndexArray().GetAt(i);
        }
        if (polySize == 3) { // is triangle
            ELGeometryVertex v1;
            ELGeometryVertex v2;
            ELGeometryVertex v3;
            FbxLoadVertex(mesh, pVertices, i, 0, v1);
            FbxLoadVertex(mesh, pVertices, i, 1, v2);
            FbxLoadVertex(mesh, pVertices, i, 2, v3);
            v1.matID = matId;
            v2.matID = matId;
            v3.matID = matId;
            vertexBuffer->append(v1, v2, v3);
        } else if (polySize == 4) {
            ELGeometryVertex v1;
            ELGeometryVertex v2;
            ELGeometryVertex v3;
            v1.matID = matId;
            v2.matID = matId;
            v3.matID = matId;
            FbxLoadVertex(mesh, pVertices, i, 0, v1);
            FbxLoadVertex(mesh, pVertices, i, 1, v2);
            FbxLoadVertex(mesh, pVertices, i, 2, v3);
            vertexBuffer->append(v1, v2, v3);
            
            FbxLoadVertex(mesh, pVertices, i, 0, v1);
            FbxLoadVertex(mesh, pVertices, i, 2, v2);
            FbxLoadVertex(mesh, pVertices, i, 3, v3);
            vertexBuffer->append(v1, v2, v3);
        }
    }
}

void ELFBXVertexBufferProvider::FbxLoadTrianglePoint(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELVector3 &pPos,ELVector2 &pUV) {
    int index = mesh->GetPolygonVertex(polyIndex, pointIndex);
    pPos.x = pVertices[index].mData[0];
    pPos.y = pVertices[index].mData[1];
    pPos.z = pVertices[index].mData[2];
    
    bool success;
    pUV = getUVFromCache(polyIndex, pointIndex, success);
    if (success) {
        return;
    }
    
    FbxVector2 texcoord;
    bool unmapped;
    FbxStringList lUVSetNameList;
    int uvCount = lUVSetNameList.GetCount();
    mesh->GetUVSetNames(lUVSetNameList);
    const char* lUVSetName = lUVSetNameList.GetStringAt(0);
    mesh->GetPolygonVertexUV(polyIndex,pointIndex,lUVSetName,texcoord,unmapped);
    pUV.x = texcoord.mData[0];
    pUV.y = texcoord.mData[1];
    
    addUVToCache(polyIndex, pointIndex, pUV);
}

void ELFBXVertexBufferProvider::FbxLoadVertex(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELGeometryVertex &vertex) {
    int index = mesh->GetPolygonVertex(polyIndex, pointIndex);
    vertex.x = pVertices[index].mData[0];
    vertex.y = pVertices[index].mData[1];
    vertex.z = pVertices[index].mData[2];
    
    vertex.clusterCount = vertexClusters[index].size();
    if (vertex.clusterCount > 0) {
        vertex.clusterID0 = vertexClusters[index].at(0).first;
        vertex.weight0 = vertexClusters[index].at(0).second;
    }
    if (vertex.clusterCount > 1) {
        vertex.clusterID1 = vertexClusters[index].at(1).first;
        vertex.weight1 = vertexClusters[index].at(1).second;
    }
    if (vertex.clusterCount > 2) {
        vertex.clusterID2 = vertexClusters[index].at(2).first;
        vertex.weight2 = vertexClusters[index].at(2).second;
    }
    if (vertex.clusterCount > 3) {
        vertex.clusterID3 = vertexClusters[index].at(3).first;
        vertex.weight3 = vertexClusters[index].at(3).second;
    }

    bool success;
    ELVector2 pUV = getUVFromCache(polyIndex, pointIndex, success);
    if (success) {
        vertex.u = pUV.x;
        vertex.v = pUV.y;
        return;
    }
    
    FbxVector2 texcoord;
    bool unmapped;
    FbxStringList lUVSetNameList;
    int uvCount = lUVSetNameList.GetCount();
    mesh->GetUVSetNames(lUVSetNameList);
    const char* lUVSetName = lUVSetNameList.GetStringAt(0);
    mesh->GetPolygonVertexUV(polyIndex,pointIndex,lUVSetName,texcoord,unmapped);
    vertex.u = texcoord.mData[0];
    vertex.v = texcoord.mData[1];
    pUV = ELVector2Make(vertex.u , vertex.v );
    
    addUVToCache(polyIndex, pointIndex, pUV);
}

void ELFBXVertexBufferProvider::addUVToCache(int polyIndex,int pointIndex, ELVector2 uv) {
    int index = polyIndex * 4 + pointIndex;
    if (index >= uvCache.size()) {
        uvCache.resize(uvCache.size() + 256);
    }
    uvCache[index] = uv;
    uvCacheAvaliableIndex = index;
}

ELVector2 ELFBXVertexBufferProvider::getUVFromCache(int polyIndex,int pointIndex, bool &success) {
    int index = polyIndex * 4 + pointIndex;
    if (index <= uvCacheAvaliableIndex) {
        success = true;
        return uvCache[index];
    }
    success = false; 
    return ELVector2Make(0, 0);
}























































std::map<std::string, ELAnimation> ELFBXVertexBufferProvider::loadAnimations() {
    std::map<std::string, ELAnimation> animations;
    for (int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); ++i) {
        ELAnimation animation;
        FbxAnimStack *stack = scene->GetSrcObject<FbxAnimStack>(i);
        animation.name = std::string(stack->GetName());
        FbxAnimLayer *animLayer = stack->GetMember<FbxAnimLayer>();

        FbxTime startTime, endTime;
        FbxTakeInfo *takeInfo = scene->GetTakeInfo(stack->GetName());
        if (takeInfo) {
            startTime = takeInfo->mLocalTimeSpan.GetStart();
            endTime = takeInfo->mLocalTimeSpan.GetStop();
            animation.startTime = startTime.GetMilliSeconds();
            animation.stopTime = endTime.GetMilliSeconds();
        }
        animations[animation.name] = animation;
    }
    return animations;
}
