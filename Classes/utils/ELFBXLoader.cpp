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
    return meshes;
}

void loadTrianglePoint(FbxMesh *mesh,FbxVector4 *pVertices, int polyIndex,int pointIndex, ELVector3 &pPos,ELVector2 &pUV) {
    int index = mesh->GetPolygonVertex(polyIndex, pointIndex);
    pPos.x = pVertices[index].mData[0];
    pPos.y = pVertices[index].mData[1];
    pPos.z = pVertices[index].mData[2];
    FbxVector4 fbxnormal;
    mesh->GetPolygonVertexNormal(polyIndex, pointIndex, fbxnormal);
    int elementUVCount = mesh->GetElementUVCount();
    for (int i = 0; i < elementUVCount; ++i) {
       FbxGeometryElementUV *elementUV = mesh->GetElementUV(i);
        switch (elementUV->GetMappingMode()) {
            case FbxGeometryElement::eByControlPoint:
                break;
            case FbxGeometryElement::eByPolygonVertex:
                switch (elementUV->GetReferenceMode()) {
                    case FbxGeometryElement::eDirect:
                        break;
                    case FbxGeometryElement::eIndexToDirect:
                        int id = elementUV->GetIndexArray().GetAt(index);
                        FbxVector2 vec2 = elementUV->GetDirectArray().GetAt(id);
                        pUV.x = vec2[0];
                        pUV.y = vec2[1];
                        break;
                }
                break;
        }
    }
}

ELMeshGeometry *ELFBXLoader::loadMesh(FbxMesh *mesh) {
    ELGeometryVertexBuffer *vertexBuffer = new ELGeometryVertexBuffer();
    int polyCount = mesh->GetPolygonCount();
    FbxVector4 *pVertices = mesh->GetControlPoints();
    for (int i = 0; i < polyCount; ++i) {
        int polySize = mesh->GetPolygonSize(i);
        if (polySize == 3) { // is triangle
            ELGeometryTriangle triangle;
            loadTrianglePoint(mesh,pVertices,i,2,triangle.point1,triangle.uv1);
            loadTrianglePoint(mesh,pVertices,i,1,triangle.point2,triangle.uv2);
            loadTrianglePoint(mesh,pVertices,i,0,triangle.point3,triangle.uv3);
            vertexBuffer->append(triangle);
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
    geometryData->vertexStride = sizeof(GLfloat) * 14;
    geometryData->vertexCount = buffer->rawLength() / sizeof(ELGeometryVertex);
    geometryData->supportIndiceVBO = false;
}