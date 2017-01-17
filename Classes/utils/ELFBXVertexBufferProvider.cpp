//
// Created by wangyang on 17/1/12.
//

#include "ELFBXVertexBufferProvider.h"
#include "ELFbxUtil.h"

ELFBXVertexBufferProvider::ELFBXVertexBufferProvider(FbxScene *scene) :
        scene(scene)
{
}

ELFBXVertexBufferProvider::~ELFBXVertexBufferProvider() {

}

void ELFBXVertexBufferProvider::prepare() {
    loadNodes();
}

void ELFBXVertexBufferProvider::loadNodes() {
    FbxNode *rootNode = scene->GetRootNode();
    if(rootNode) {
        for(int i = 0; i < rootNode->GetChildCount(); i++) {
            FbxNode *node = rootNode->GetChild(i);
            if (node->GetNodeAttribute() == NULL) {
                continue;
            }
            FbxNodeAttribute::EType attrType = node->GetNodeAttribute()->GetAttributeType();
            if (attrType == FbxNodeAttribute::EType::eMesh) {
                mesh = node->GetMesh();
            }
        }
    }
    int poseCount = scene->GetPoseCount();
    currentPose = scene->GetPose(0);
}

void ELFBXVertexBufferProvider::update(ELFloat timeInSecs, ELGeometryVertexBuffer *vertexBuffer) {
    update("shot", timeInSecs,vertexBuffer);
}

void ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, FbxVector4* pVertexArray)
{

    int lVertexCount = pMesh->GetControlPointsCount();

    FbxVector4* lSrcVertexArray = pVertexArray;
    FbxVector4* lDstVertexArray = new FbxVector4[lVertexCount];
    memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));

    int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
    for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
    {
        printf("%s\n", "come the shapes");
        FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

        int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
        for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
        {
            FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
            if(lChannel)
            {
                // Get the percentage of influence on this channel.
                FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
                if (!lFCurve) continue;
                double lWeight = lFCurve->Evaluate(pTime);

                /*
                If there is only one targetShape on this channel, the influence is easy to calculate:
                influence = (targetShape - baseGeometry) * weight * 0.01
                dstGeometry = baseGeometry + influence

                But if there are more than one targetShapes on this channel, this is an in-between
                blendshape, also called progressive morph. The calculation of influence is different.

                For example, given two in-between targets, the full weight percentage of first target
                is 50, and the full weight percentage of the second target is 100.
                When the weight percentage reach 50, the base geometry is already be fully morphed
                to the first target shape. When the weight go over 50, it begin to morph from the
                first target shape to the second target shape.

                To calculate influence when the weight percentage is 25:
                1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
                2. And since 25 is already half way between 0 and 50, so the real weight percentage change to
                the first target is 50.
                influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
                dstGeometry = baseGeometry + influence

                To calculate influence when the weight percentage is 75:
                1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
                2. And since 75 is already half way between 50 and 100, so the real weight percentage change
                to the second target is 50.
                influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
                dstGeometry = firstTargetShape + influence
                */

                // Find the two shape indices for influence calculation according to the weight.
                // Consider index of base geometry as -1.

                int lShapeCount = lChannel->GetTargetShapeCount();
                double* lFullWeights = lChannel->GetTargetShapeFullWeights();

                // Find out which scope the lWeight falls in.
                int lStartIndex = -1;
                int lEndIndex = -1;
                for(int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
                {
                    if(lWeight > 0 && lWeight <= lFullWeights[0])
                    {
                        lEndIndex = 0;
                        break;
                    }
                    if(lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex+1])
                    {
                        lStartIndex = lShapeIndex;
                        lEndIndex = lShapeIndex + 1;
                        break;
                    }
                }

                FbxShape* lStartShape = NULL;
                FbxShape* lEndShape = NULL;
                if(lStartIndex > -1)
                {
                    lStartShape = lChannel->GetTargetShape(lStartIndex);
                }
                if(lEndIndex > -1)
                {
                    lEndShape = lChannel->GetTargetShape(lEndIndex);
                }

                //The weight percentage falls between base geometry and the first target shape.
                if(lStartIndex == -1 && lEndShape)
                {
                    double lEndWeight = lFullWeights[0];
                    // Calculate the real weight.
                    lWeight = (lWeight/lEndWeight) * 100;
                    // Initialize the lDstVertexArray with vertex of base geometry.
                    memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
                    for (int j = 0; j < lVertexCount; j++)
                    {
                        // Add the influence of the shape vertex to the mesh vertex.
                        FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
                        lDstVertexArray[j] += lInfluence;
                    }
                }
                    //The weight percentage falls between two target shapes.
                else if(lStartShape && lEndShape)
                {
                    double lStartWeight = lFullWeights[lStartIndex];
                    double lEndWeight = lFullWeights[lEndIndex];
                    // Calculate the real weight.
                    lWeight = ((lWeight-lStartWeight)/(lEndWeight-lStartWeight)) * 100;
                    // Initialize the lDstVertexArray with vertex of the previous target shape geometry.
                    memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
                    for (int j = 0; j < lVertexCount; j++)
                    {
                        // Add the influence of the shape vertex to the previous shape vertex.
                        FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
                        lDstVertexArray[j] += lInfluence;
                    }
                }
            }//If lChannel is valid
        }//For each blend shape channel
    }//For each blend shape deformer

    memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));

    delete [] lDstVertexArray;
}

void ELFBXVertexBufferProvider::update(const char *animationName,ELFloat elapsedTime, ELGeometryVertexBuffer *vertexBuffer) {
    FbxTime fbxElapsedTime(FbxLongLong(FBXSDK_TC_SECOND*elapsedTime));

    int vertexCount = mesh->GetControlPointsCount();
    FbxVector4 *vertexArray = new FbxVector4[vertexCount];
    memcpy(vertexArray, mesh->GetControlPoints(), vertexCount * sizeof(FbxVector4));

    FbxAnimStack * lCurrentAnimationStack = scene->FindMember<FbxAnimStack>(animationName);

    // we assume that the first animation layer connected to the animation stack is the base layer
    // (this is the assumption made in the FBXSDK)
    FbxAnimLayer *mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
    scene->SetCurrentAnimationStack(lCurrentAnimationStack);

    int shapeCount = mesh->GetShapeCount();
    const bool lHasShape =shapeCount > 0;
    if (lHasShape) {
        ComputeShapeDeformation(mesh, fbxElapsedTime, mCurrentAnimLayer, vertexArray);
    }

    int clusterCount = 0;
    for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); ++i) {
        FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(i, FbxDeformer::eSkin);
        clusterCount += skin->GetClusterCount();
    }
    if (clusterCount) {
        FbxSkin * skin = (FbxSkin *)mesh->GetDeformer(0, FbxDeformer::eSkin);
        FbxSkin::EType skinType = skin->GetSkinningType();
        if (skinType == FbxSkin::eLinear || skinType == FbxSkin::eRigid) {
            FbxAMatrix globalMatrix = FbxGetNodeGlobalPosition(mesh->GetNode(), fbxElapsedTime, currentPose, NULL);
            computerLinearDeformation(globalMatrix, mesh, fbxElapsedTime, vertexArray);
            loadVerticesToVertexBuffer(mesh, vertexArray, vertexBuffer);
        }
    }
}

void ELFBXVertexBufferProvider::computerLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxTime time, FbxVector4 *vertexArray) {


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
//
//    printf("\n");
//    static int in = 0;
//    if (in < 20) {
////                        printf("DST==> %f %f %f %f\n",  vertexArray[i].mData[0], vertexArray[i].mData[1], vertexArray[i].mData[2], vertexArray[i].mData[3]);
//        printf("Weight: %f", clusterWeight[1]);
//        in++;
//    }

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

                static int in = 0;
                if (in < 20) {
                    in ++;
                    printf("DST==> %f %f %f %f\n",  lDstVertex.mData[0], lDstVertex.mData[1], lDstVertex.mData[2], lDstVertex.mData[3]);
                    // printf("Weight: %f", lClusterWeight[in]);
                }
            }
            else if (clusterMode == FbxCluster::eTotalOne) {
                // In the total 1 link mode, a vertex can be partially influenced by the links.
                lSrcVertex *= (1.0 - lWeight);
                lDstVertex += lSrcVertex;
            }
        }
    }

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
            ELGeometryTriangle triangle;
            FbxLoadTrianglePoint(mesh,pVertices,i,0,triangle.point3,triangle.uv3);
            FbxLoadTrianglePoint(mesh,pVertices,i,1,triangle.point2,triangle.uv2);
            FbxLoadTrianglePoint(mesh,pVertices,i,2,triangle.point1,triangle.uv1);
            vertexBuffer->append(triangle, matId);
        }
    }
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