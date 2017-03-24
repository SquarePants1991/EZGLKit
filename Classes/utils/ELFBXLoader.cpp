//
// Created by wangyang on 16/12/30.
//

#include "ELFBXLoader.h"
#include "ELAssets.h"
#include "ELGeometryVertexBuffer.h"
#include "../types/EZGLVectorTypes.h"
#include "../types/ELGeometryTypes.h"
#include "../component/geometry/ELMeshGeometry.h"
#include "ELFbxUtil.h"
#include "ELFBXVertexBufferProvider.h"
#include "../core/ELTexture.h"
#include <fbxsdk.h>

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

    FbxNode *rootNode = scene->GetRootNode();
    if(rootNode) {
        for(int i = 0; i < rootNode->GetChildCount(); i++) {
            FbxNode *node = rootNode->GetChild(i);
            if (node->GetNodeAttribute() == NULL) {
                continue;
            }
            FbxNodeAttribute::EType attrType = node->GetNodeAttribute()->GetAttributeType();
            if (attrType == FbxNodeAttribute::EType::eMesh) {
                FbxMesh *mesh = node->GetMesh();
                FbxAMatrix transform = FbxGetNodeGlobalPosition(node, NULL, NULL);
                ELFBXVertexBufferProvider *vertexBufferProvider = new ELFBXVertexBufferProvider(scene, mesh);
                vertexBufferProvider->prepare();
                
                ELMeshGeometry *meshGeometry = new ELMeshGeometry();
                FbxVector4 pos = transform.GetT();
                meshGeometry->transform->position.x = pos[0];
                meshGeometry->transform->position.y = pos[1];
                meshGeometry->transform->position.z = pos[2];
                
                FbxVector4 scale = transform.GetS();
                meshGeometry->transform->scale.x = scale[0];
                meshGeometry->transform->scale.y = scale[1];
                meshGeometry->transform->scale.z = scale[2];
                
                FbxQuaternion rotate = transform.GetQ();
                meshGeometry->transform->quaternion.x = rotate[0];
                meshGeometry->transform->quaternion.y = rotate[1];
                meshGeometry->transform->quaternion.z = rotate[2];
                 meshGeometry->transform->quaternion.w = rotate[3];
                
                meshGeometry->vertexBufferProvider = vertexBufferProvider;
                meshGeometry->animations = vertexBufferProvider->loadAnimations();
                loadMaterial(meshGeometry, mesh);
                meshes.push_back(meshGeometry);
            }
        }
    }

    return meshes;
}

void ELFBXLoader::loadMaterial(ELMeshGeometry *geometry, FbxMesh *mesh) {
    FbxNode *pNode = mesh->GetNode();
    const int lMaterialCount = pNode->GetMaterialCount();
    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
    {
        FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
        if (lMaterial)
        {
            FbxProperty prop = lMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
            int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
            
            if (layeredTextureCount > 0)
            {
                for (int j = 0; j < layeredTextureCount; j++)
                {
                    FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
                    int lcount = layered_texture->GetSrcObjectCount<FbxFileTexture>();
                    
                    for (int k = 0; k < lcount; k++)
                    {
                        FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxFileTexture>(k));
                        // Then, you can get all the properties of the texture, include its name
                        const char* textureName = texture->GetName();
                    }
                }
            }
            else
            {
                // Directly get textures
                int textureCount = prop.GetSrcObjectCount<FbxFileTexture>();
                for (int j = 0; j < textureCount; j++)
                {
                    FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
                    // Then, you can get all the properties of the texture, include its name
                    const char* textureName = texture->GetName();
                    
                    const char *fileName = texture->GetFileName();
                    geometry->materials[lMaterialIndex].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1);
                    geometry->materials[lMaterialIndex].diffuseMap = (ELUint)ELTexture::texture(ELAssets::shared()->findFile(std::string(fileName)))->value;
                }
            }
        }
    }
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
