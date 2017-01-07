//
// Created by wangyang on 16/12/30.
//

#include "ELFBXLoader.h"
#include "ELAssets.h"
#include "ELGeometryVertexBuffer.h"
#include <types/EZGLVectorTypes.h>
#include <types/ELGeometryTypes.h>
#include "component/geometry/ELMeshGeometry.h"

FbxManager * ELFBXLoader::fbxManager = NULL;

void ELFBXLoader::setupFbxEnv() {
    if (ELFBXLoader::fbxManager == NULL) {
        ELFBXLoader::fbxManager = FbxManager::Create();
        FbxIOSettings *ios = FbxIOSettings::Create(ELFBXLoader::fbxManager, IOSROOT);
        ELFBXLoader::fbxManager->SetIOSettings(ios);
    }
}

std::vector<ELMeshGeometry *> ELFBXLoader::loadFromFile(const char *filePath) {
    setupFbxEnv();
    std::vector<ELMeshGeometry *> meshes;

    FbxImporter *importer = FbxImporter::Create(ELFBXLoader::fbxManager,"");
    bool res = importer->Initialize(filePath, -1, ELFBXLoader::fbxManager->GetIOSettings());
    if (!res) {
        printf("import error: %s", importer->GetStatus().GetErrorString());
    }

    FbxScene *scene = FbxScene::Create(ELFBXLoader::fbxManager,"scene");
    importer->Import(scene);
    importer->Destroy();

    FbxAxisSystem::OpenGL.ConvertScene(scene);

    FbxNode* rootNode = scene->GetRootNode();
    if(rootNode) {
        for(int i = 0; i < rootNode->GetChildCount(); i++) {
            FbxNode *node = rootNode->GetChild(i);
            if (node->GetNodeAttribute() == NULL) {
                continue;
            }
            FbxNodeAttribute::EType attrType = node->GetNodeAttribute()->GetAttributeType();
            if (attrType == FbxNodeAttribute::EType::eMesh) {
                FbxMesh * mesh = (FbxMesh *) node->GetNodeAttribute();
                ELMeshGeometry *meshGeometry = loadMesh(mesh);
                meshGeometry->animations = loadAnimationNames(scene);
                meshes.push_back(meshGeometry);
            }
        }
    }

    return meshes;
}

void loadTrianglePoint(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELVector3 &pPos,ELVector2 &pUV) {
    int index = mesh->GetPolygonVertex(polyIndex, pointIndex);
    pPos.x = pVertices[index].mData[0];
    pPos.y = pVertices[index].mData[1];
    pPos.z = pVertices[index].mData[2];

    FbxVector2 texcoord;
    bool unmapped;
    FbxStringList lUVSetNameList;
    int uvCount = lUVSetNameList.GetCount();
    mesh->GetUVSetNames(lUVSetNameList);
    const char* lUVSetName = lUVSetNameList.GetStringAt(0);
    mesh->GetPolygonVertexUV(polyIndex,pointIndex,lUVSetName,texcoord,unmapped);
    pUV.x = texcoord.mData[0];
    pUV.y = 1 - texcoord.mData[1];
}

ELMeshGeometry *ELFBXLoader::loadMesh(FbxMesh *mesh) {
    ELGeometryVertexBuffer *vertexBuffer = new ELGeometryVertexBuffer();
    int polyCount = mesh->GetPolygonCount();
    FbxVector4 *pVertices = mesh->GetControlPoints();
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

        // TODO: add rectangle support
        if (polySize == 3) { // is triangle
            ELGeometryTriangle triangle;
            loadTrianglePoint(mesh,pVertices,i,0,triangle.point3,triangle.uv3);
            loadTrianglePoint(mesh,pVertices,i,1,triangle.point2,triangle.uv2);
            loadTrianglePoint(mesh,pVertices,i,2,triangle.point1,triangle.uv1);
            vertexBuffer->append(triangle, matId);
        }
    }
    ELGeometryData geometryData;
    generateVertexVBO(vertexBuffer, &geometryData);

    ELMeshGeometry *meshGeometry = new ELMeshGeometry(geometryData);
    return meshGeometry;
}

void ELFBXLoader::generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData) {
    GLuint vertexVBO;
    GLfloat *pVertices = (GLfloat *)(buffer->data());
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, buffer->rawLength(), pVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    geometryData->vertexVBO = vertexVBO;
    geometryData->vertexStride = sizeof(ELGeometryVertex);
    geometryData->vertexCount = buffer->rawLength() / sizeof(ELGeometryVertex);
    geometryData->supportIndiceVBO = false;
}

std::map<std::string, ELAnimation> ELFBXLoader::loadAnimationNames(FbxScene *scene) {
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

void ELFBXLoader::loadAnimation(FbxScene *scene) {
    for (int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); ++i) {
        FbxAnimStack *stack = scene->GetSrcObject<FbxAnimStack>(i);
        printf("Anim Stack : %s \n", stack->GetName());

        FbxAnimLayer *animLayer = stack->GetMember<FbxAnimLayer>();

        FbxTime startTime, endTime;
        FbxTakeInfo *takeInfo = scene->GetTakeInfo(stack->GetName());
        if (takeInfo) {
            startTime = takeInfo->mLocalTimeSpan.GetStart();
            endTime = takeInfo->mLocalTimeSpan.GetStop();
            printf("%d s : %d s \n",startTime.GetSecondCount(), endTime.GetSecondCount());
        }
    }

    //check if any pose exists
    int posCount = scene->GetPoseCount();
    FbxPose *pose = scene->GetPose(0);

    // check if any cache exists
    int nodeCount = scene->GetSrcObjectCount<FbxNode>();
    for (int j = 0; j < nodeCount; ++j) {
        FbxNode *node = scene->GetSrcObject<FbxNode>(j);
        if (node->GetGeometry()) {
            int vertexCacheDeformerCount = node->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);
            for (int i = 0; i < vertexCacheDeformerCount; ++i) {
                FbxVertexCacheDeformer *deformer = static_cast<FbxVertexCacheDeformer *>(node->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
            }
        }
    }
}

void ELFBXLoader::loadSkin(FbxMesh *mesh) {
    int shapeCount = mesh->GetShapeCount();
    int clusterCount = 0;
    for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); ++i) {
        FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(i, FbxDeformer::eSkin);
        clusterCount += skin->GetClusterCount();
    }
    if (clusterCount) {
        FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(0, FbxDeformer::eSkin);
        FbxSkin::EType skinType = skin->GetSkinningType();
        if (skinType == FbxSkin::eLinear || skinType == FbxSkin::eRigid) {
            FbxAMatrix globalMatrix;
            computerLinearDeformation(globalMatrix, mesh, FbxTime());
        }
    }
}

FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
{
    FbxAMatrix lPoseMatrix;
    FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

    memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

    return lPoseMatrix;
}

FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition = NULL)
{
    FbxAMatrix lGlobalPosition;
    bool        lPositionFound = false;

    if (pPose)
    {
        int lNodeIndex = pPose->Find(pNode);

        if (lNodeIndex > -1)
        {
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
            if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
            {
                lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
            }
            else
            {
                // We have a local matrix, we need to convert it to
                // a global space matrix.
                FbxAMatrix lParentGlobalPosition;

                if (pParentGlobalPosition)
                {
                    lParentGlobalPosition = *pParentGlobalPosition;
                }
                else
                {
                    if (pNode->GetParent())
                    {
                        lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
                    }
                }

                FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
                lGlobalPosition = lParentGlobalPosition * lLocalPosition;
            }

            lPositionFound = true;
        }
    }

    if (!lPositionFound)
    {
        // There is no pose entry for that node, get the current global position instead.

        // Ideally this would use parent global position and local position to compute the global position.
        // Unfortunately the equation
        //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
        // does not hold when inheritance type is other than "Parent" (RSrs).
        // To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
        lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
    }

    return lGlobalPosition;
}

// Scale all the elements of a matrix.
void MatrixScale(FbxAMatrix& pMatrix, double pValue)
{
    int i,j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            pMatrix[i][j] *= pValue;
        }
    }
}

void ELFBXLoader::computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime time) {
    FbxCluster::ELinkMode clusterMode = ((FbxSkin *)mesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
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
            FbxTime time;
            computerClusterDeformation(pGlobalPosition, time, NULL, mesh, cluster, vertexTransformMatrix);
            int lVertexIndexCount = cluster->GetControlPointIndicesCount();
            for (int k = 0; k < lVertexIndexCount; ++k)
            {
                int lIndex = cluster->GetControlPointIndices()[k];

                // Sometimes, the mesh can have less points than at the time of the skinning
                // because a smooth operator was active when skinning but has been deactivated during export.
                if (lIndex >= vertexCount)
                    continue;

                double lWeight = cluster->GetControlPointWeights()[k];

                if (lWeight == 0.0)
                {
                    continue;
                }

                // Compute the influence of the link on the vertex.
                FbxAMatrix lInfluence = vertexTransformMatrix;
                MatrixScale(lInfluence, lWeight);

                if (clusterMode == FbxCluster::eAdditive)
                {
//                    // Multiply with the product of the deformations on the vertex.
//                    MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
//                    lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
//
//                    // Set the link to 1.0 just to know this vertex is influenced by a link.
//                    lClusterWeight[lIndex] = 1.0;
                }
                else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
                {
//                    // Add to the sum of the deformations on the vertex.
//                    MatrixAdd(lClusterDeformation[lIndex], lInfluence);
//
//                    // Add to the sum of weights to either normalize or complete the vertex.
//                    lClusterWeight[lIndex] += lWeight;
                }
            }//For each vertex

            //Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
            for (int i = 0; i < vertexCount; i++)
            {
                FbxVector4 lSrcVertex = pVertexArray[i];
                FbxVector4& lDstVertex = pVertexArray[i];
                double lWeight = lClusterWeight[i];

                // Deform the vertex if there was at least a link with an influence on the vertex,
                if (lWeight != 0.0)
                {
                    lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
                    if (lClusterMode == FbxCluster::eNormalize)
                    {
                        // In the normalized link mode, a vertex is always totally influenced by the links.
                        lDstVertex /= lWeight;
                    }
                    else if (lClusterMode == FbxCluster::eTotalOne)
                    {
                        // In the total 1 link mode, a vertex can be partially influenced by the links.
                        lSrcVertex *= (1.0 - lWeight);
                        lDstVertex += lSrcVertex;
                    }
                }
            }
        }
    }
}

FbxAMatrix GetGeometry(FbxNode* pNode)
{
    const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(lT, lR, lS);
}

void ELFBXLoader::computerClusterDeformation(FbxAMatrix& pGlobalPosition,FbxTime &pTime, FbxPose *pPose, FbxMesh *pMesh, FbxCluster *pCluster, FbxAMatrix& pVertexTransformMatrix) {
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
        lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
        lAssociateGlobalInitPosition *= lAssociateGeometry;
        lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

        pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
        // Mesh Matrix
        // Multiply lReferenceGlobalInitPosition by Geometric Transformation
        lReferenceGeometry = GetGeometry(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;
        lReferenceGlobalCurrentPosition = pGlobalPosition;

        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        // Multiply lClusterGlobalInitPosition by Geometric Transformation
        lClusterGeometry = GetGeometry(pCluster->GetLink());
        lClusterGlobalInitPosition *= lClusterGeometry;
        lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

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
        lReferenceGeometry = GetGeometry(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;

        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

        // Compute the initial position of the link relative to the reference.
        lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

        // Compute the current position of the link relative to the reference.
        lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

        // Compute the shift of the link relative to the reference.
        pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
    }
}