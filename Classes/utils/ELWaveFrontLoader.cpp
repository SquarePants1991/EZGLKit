//
// Created by wangyang on 16/11/28.
//

#include "wavefront/tiny_obj_loader.h"
#include "../component/ELGeometry.h"
#include "../core/ELTexture.h"
#include "ELWaveFrontLoader.h"
#include "../component/geometry/ELMeshGeometry.h"
#include "ELAssets.h"

static std::vector<ELMaterial> loadMaterials(std::vector<tinyobj::material_t> materials);
static ELMeshGeometry * buildGeometry(tinyobj::shape_t shape, tinyobj::attrib_t attrib, std::vector<ELMaterial> materials);
static ELGeometryVertexBuffer *generateVertexBuffer(tinyobj::attrib_t attrib, std::vector<tinyobj::index_t> indices);
static void generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData);

std::vector<ELMeshGeometry *> ELWaveFrontLoader::loadFile(std::string filePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::size_t loc = filePath.rfind("/");
    std::string resourcePath;
    if (loc != std::string::npos) {
        resourcePath = filePath.substr(0,loc + 1);
    }
    std::string err;
    tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(),resourcePath.c_str());

    if (!err.empty()) { // `err` may contain warning message.
        printf("%s", err.c_str());
    }

    std::vector<GLfloat> vertices;
    std::vector<tinyobj::index_t> indices;
    std::vector<ELMaterial> elMaterials = loadMaterials(materials);
    std::vector<ELMeshGeometry *> geometries;
    for (size_t s = 0; s < shapes.size(); s++) {
        ELMeshGeometry * geometry = buildGeometry(shapes.at(s), attrib, elMaterials);
        geometries.push_back(geometry);
    }
    return geometries;
}

static std::vector<ELMaterial> loadMaterials(std::vector<tinyobj::material_t> materials) {
    std::vector<ELMaterial> mats;
    for (size_t index = 0; index < materials.size(); index++) {
        ELMaterial material;
        tinyobj::material_t material_t = materials.at(index);
        material.ambient = ELVector4Make(material_t.ambient[0], material_t.ambient[1], material_t.ambient[2], 1.0);
        material.specular = ELVector4Make(material_t.specular[0], material_t.specular[1], material_t.specular[2], 1.0);
        material.diffuse = ELVector4Make(material_t.diffuse[0], material_t.diffuse[1], material_t.diffuse[2], 1.0);
        material.ambientMap = ELTexture::texture(ELAssets::shared()->findFile(material_t.diffuse_texname))->value;
        material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile(material_t.diffuse_texname))->value;
        material.specularMap = ELTexture::texture(ELAssets::shared()->findFile(material_t.specular_texname))->value;
        material.normalMap = ELTexture::texture(ELAssets::shared()->findFile(material_t.bump_texname))->value;
        mats.push_back(material);
    }
    return mats;
}

static ELMeshGeometry * buildGeometry(tinyobj::shape_t shape, tinyobj::attrib_t attrib, std::vector<ELMaterial> materials) {
    std::vector<tinyobj::index_t> indices;
    for (size_t index = 0; index < shape.mesh.indices.size(); index++) {
        tinyobj::index_t indice = shape.mesh.indices[index];
        indices.push_back(indice);
    }
    ELGeometryData geometryData;
    ELGeometryVertexBuffer *buffer = generateVertexBuffer(attrib, indices);
    buffer->caculatePerVertexNormal();
    generateVertexVBO(buffer, &geometryData);
    ELMeshGeometry * geometry = new ELMeshGeometry(geometryData);
    if (shape.mesh.material_ids[0] >= 0 && shape.mesh.material_ids[0] < materials.size()) {
        geometry->material = materials[shape.mesh.material_ids[0]];
    }
    return geometry;
}

static  ELGeometryVertexBuffer * generateVertexBuffer(tinyobj::attrib_t attrib, std::vector<tinyobj::index_t> indices) {
    ELGeometryVertexBuffer *buffer = new ELGeometryVertexBuffer();

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> tangents;
    std::vector<GLfloat> bitangents;

    for (size_t i = 0; i < indices.size(); i++) {
        tinyobj::index_t indice = indices[i];
        GLfloat x = attrib.vertices[indice.vertex_index * 3 + 0];
        GLfloat y = attrib.vertices[indice.vertex_index * 3 + 1];
        GLfloat z = attrib.vertices[indice.vertex_index * 3 + 2];
        GLfloat u = attrib.texcoords[indice.texcoord_index * 2 + 0];
        GLfloat v = attrib.texcoords[indice.texcoord_index * 2 + 1];
        GLfloat nx = attrib.normals[indice.normal_index * 3 + 0];
        GLfloat ny = attrib.normals[indice.normal_index * 3 + 1];
        GLfloat nz = attrib.normals[indice.normal_index * 3 + 2];

        ELGeometryVertex vertex = {x,y,z,nx,ny,nz,u,v};
        buffer->append(vertex);
    }

    buffer->caculatePerVertexNormal();

    return buffer;
}

static void generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData) {
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
