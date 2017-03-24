//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELWAVEFRONTLOADER_H
#define EZGL_ELWAVEFRONTLOADER_H

#include "../component/ELGeometry.h"
#include "wavefront/tiny_obj_loader.h"
#include "ELGeometryVertexBuffer.h"

#include <vector>
#include <string>

class ELMeshGeometry;

class ELWaveFrontLoader {
public:
    static std::vector<ELMeshGeometry *> loadFile(std::string filePath);

private:
    static std::vector<ELMaterial> loadMaterials(std::vector<tinyobj::material_t> materials);
    static ELMeshGeometry * buildGeometry(tinyobj::shape_t shape, tinyobj::attrib_t attrib, std::vector<ELMaterial> materials);
    static ELGeometryVertexBuffer *generateVertexBuffer(tinyobj::attrib_t attrib, std::vector<tinyobj::index_t> indices);
    static void generateVertexVBO(ELGeometryVertexBuffer *buffer, ELGeometryData * geometryData);
};


#endif //EZGL_ELWAVEFRONTLOADER_H
