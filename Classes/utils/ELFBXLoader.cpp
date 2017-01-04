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
        printf("Anim Stack : %s\n", stack->GetName());

        int animLayersCount = stack->GetMemberCount<FbxAnimLayer>();
        for (int j = 0; j < animLayersCount; ++j) {
            FbxAnimLayer *layer = stack->GetMember<FbxAnimLayer>(j);
            FbxAnimCurve * curveX = scene->GetRootNode()->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
        }
    }
}