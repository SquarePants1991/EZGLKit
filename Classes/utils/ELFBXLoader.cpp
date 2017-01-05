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
                loadSkin(mesh);
                meshes.push_back(loadMesh(mesh));
            }
        }
    }
    loadAnimation(scene);
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
            computerClusterDeformation(pGlobalPosition, mesh, cluster, vertexTransformMatrix);
        }
    }
}

void ELFBXLoader::computerClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh *mesh, FbxCluster *cluster, FbxAMatrix& vertexTransform) {

}